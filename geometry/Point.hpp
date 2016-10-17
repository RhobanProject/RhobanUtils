#ifndef POINT_HPP
#define POINT_HPP

#include <iostream>
#include <vector>

#include "Angle.hpp"

#ifdef ENABLE_OPENCV
#include <opencv2/core/core.hpp>
#endif

class Point{
public:
  double x;
  double y;
  Point(): x(0.0), y(0.0) {};
  Point(const Point & other): x(other.x), y(other.y) {};
  Point(double x, double y): x(x), y(y) {};

#ifdef ENABLE_OPENCV
  Point(const cv::Point & p): x(p.x), y(p.y){} ;
  Point(const cv::Point2f & p): x(p.x), y(p.y){} ;

  cv::Point toCV() const { return cv::Point((int)x, (int)y);}
  cv::Point2f toCV2f() const { return cv::Point2f(x, y);}
#endif

  double getX() const{ return x;};
  double getY() const{ return y;};

//TODO handle case with point at 0.0, 0.0 in getTheta and isCollinear
  Angle getTheta() const;
  bool isCollinear(const Point & other) const;
  /* Return the distance to point in {0,0} */
  double getLength() const;
  double getDist(const Point & other) const;
  Point & normalize(double newLength = 1);

  /* Return a Point with a rotation of 90° */
  Point perpendicular() const;
  Point rotation(const Angle & a) const;
  Point rotation(const Angle & a,
                 const Point & rotCenter) const;

  static double dotProduct(const Point & p1, const Point & p2);
  static double perpDotProduct(const Point & p1, const Point & p2);

  Point operator-() const;

  Point operator+(const Point & other) const;
  Point operator-(const Point & other) const;
  Point operator*(double ratio) const;
  Point operator/(double ratio) const;

  Point& operator+=(const Point & other);
  Point& operator-=(const Point & other);
  Point& operator*=(double ratio);
  Point& operator/=(double ratio);

  static Point mkPointFromPolar(double rho, Angle theta);

  // Random points creators
  static Point mkRandomPolar(double rhoMax);
  static Point mkRandomPolar(double rhoMin, double rhoMax,
                             double thetaMin =   0.0,
                             double thetaMax = 360.0);

  bool operator==(const Point & other) const;
};

Point operator*(double ratio, const Point & p);

std::ostream& operator<<(std::ostream& out, const Point& p);

/* Return the point which is the average of the points in the vector */
Point average(const std::vector<Point> & points);
/* Return sum(points[i]* weights[i]) / sum(weights[i]) */
Point average(const std::vector<Point> & points, const std::vector<double> & weights);
/* Return the standard deviation to the average of the points */
double stdDev(const std::vector<Point> & points);

#endif//POINT_HPP
