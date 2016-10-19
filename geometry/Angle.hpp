#ifndef ANGLE_HPP
#define ANGLE_HPP

#include <vector>
#include <ostream>

#ifdef ENABLE_OPENCV
#include <opencv2/core/core.hpp>
#endif

/* This class aims to handle most of the frequent problems when manipulating
 * angles. In this class, if not specified, values are always between 0 and 
 * 360.
 */
class Angle{
protected:
  double value;
public:
  Angle(): value(0.0) {};
  Angle(double a);//a must be given in degrees
  Angle(const Angle & other): value(other.value) {};

  /* Value in [0, 360] */
  double getValue() const;
  /* Value in [-180,180] */
  double getSignedValue() const;

  static Angle fromXY(double x, double y);
  static Angle mean(const std::vector<Angle>& angles);
  static Angle mean(const std::vector<Angle>& angles, double * stdDev);
  static double stdDev(const std::vector<Angle>& angles);

  static Angle arcsin(double x);
  static Angle arccos(double x);
  static Angle weightedAverage(const Angle & a1, double w1,
                               const Angle & a2, double w2);

  bool equals(const Angle & other) const;

  Angle operator-(const Angle & a) const;
  Angle operator+(const Angle & a) const;

#ifdef ENABLE_OPENCV
  static Angle angleBetween(const cv::Point3f & v1, const cv::Point3f & v2)
    {
      double nv1 = norm(v1);
      double nv2 = norm(v2);
      if (nv1 < 1e-9 || nv2 < 1e-9) {
        return 0;
      }
      double x = v1.dot(v2) / (nv1 * nv2);
      if (x > 1.0) { x = 1.0;}
      if (x < -1.0) { x = -1.0;}
      return arccos(x);
    }
#endif

  bool operator==(const Angle & a) const;
};

Angle operator-(const Angle & a);
Angle operator*(double x, const Angle& a);
Angle operator*(const Angle& a, double x);
Angle operator/(const Angle& a, double x);

std::ostream& operator<<(std::ostream& out, const Angle& a);

double sin(const Angle & a);
double cos(const Angle & a);
double tan(const Angle & a);

double deg2rad(double degreeAngle);
double rad2deg(double radAngle);

#endif//ANGLE_HPP
