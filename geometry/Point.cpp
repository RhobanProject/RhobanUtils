#include <math.h>
#include <cmath>

#include "Point.hpp"

#include <random>
#include <chrono>

#define EPSILON 0.000001

using namespace std;

std::default_random_engine get_random_engine()
{
  unsigned long seed = std::chrono::system_clock::now().time_since_epoch().count();
  return std::default_random_engine(seed);
}


double Point::getDist(const Point & other) const{
  double dx = getX() - other.getX();
  double dy = getY() - other.getY();
  return sqrt(dx * dx + dy * dy);
}

double Point::getLength() const{
  static Point zero(0.0,0.0);
  return getDist(zero);
}

Point & Point::normalize(double newLength){
  if (newLength == 0) {
      x = 0;
      y = 0;
  } else {
      double denominator = getLength() / newLength;
      x /= denominator;
      y /= denominator;
  }
  return *this;
}

Angle Point::getTheta() const{
  double theta = atan2(getY(), getX()) * 180 / M_PI;
  return Angle(theta);
}

bool Point::isCollinear(const Point & other) const{
  Angle deltaTheta = getTheta() - other.getTheta();
  double delta = abs(deltaTheta.getSignedValue());
  return (delta < EPSILON ||
          (180 - delta) < EPSILON);
}

Point Point::perpendicular() const{
  double newX = -getY();
  double newY = getX();
  return Point(newX, newY);
}

Point Point::rotation(const Angle & a) const{
  double newX = getX() * cos(a) - getY() * sin(a);
  double newY = getX() * sin(a) + getY() * cos(a);
  return Point(newX,newY);
}

Point Point::rotation(const Angle & a, const Point & rotCenter) const{
  Point diff = *this - rotCenter;
  return rotCenter + diff.rotation(a);
}

Point Point::operator-() const{
  return Point(- getX(), - getY());
}

double Point::dotProduct(const Point & p1, const Point & p2){
  return p1.getX() * p2.getX() + p1.getY() * p2.getY();
}

double Point::perpDotProduct(const Point & p1, const Point & p2){
  return p1.getX() * p2.getY() - p1.getY() * p2.getX();
}

Point Point::operator+(const Point & other) const{
  return Point(getX() + other.getX(), getY() + other.getY());
}

Point Point::operator-(const Point & other) const{
  return Point(getX() - other.getX(), getY() - other.getY());
}

Point Point::operator*(double ratio) const{
  return Point(getX() * ratio, getY() * ratio);
}

Point Point::operator/(double ratio) const{
  return Point(getX() / ratio, getY() / ratio);
}

Point& Point::operator+=(const Point & other)
{
  *this = *this + other;
  return *this;
}

Point& Point::operator-=(const Point & other)
{
  *this = *this - other;
  return *this;
}

Point& Point::operator*=(double ratio)
{
  *this = *this * ratio;
  return *this;
}
Point& Point::operator/=(double ratio)
{
  *this = *this / ratio;
  return *this;
}

bool Point::operator==(const Point & other) const{
  bool sameX = getX() == other.getX();
  bool sameY = getY() == other.getY();
  return sameX && sameY;
}


Point Point::mkPointFromPolar(double rho, Angle a){
  double theta = a.getValue() * M_PI / 180.0;
  return Point(cos(theta) * rho, sin(theta) * rho);
}

Point Point::mkRandomPolar(double rhoMax)
{
  return mkRandomPolar(0.0, rhoMax);
}

Point Point::mkRandomPolar(double rhoMin, double rhoMax,
                           double thetaMin, double thetaMax){
  auto generator = get_random_engine();
  std::uniform_real_distribution<double> rhoDist(rhoMin, rhoMax);
  std::uniform_real_distribution<double> thetaDist(thetaMin, thetaMax);
  double rho = rhoDist(generator);
  double theta = thetaDist(generator);
  return mkPointFromPolar(rho, theta);
}

Point operator*(double ratio, const Point & p)
{
  return p * ratio;
}

ostream& operator<<(ostream& out, const Point& p){
  return out << '{' << p.getX() << ',' << p.getY() << '}';
}

Point average(const vector<Point> & points)
{
  double xSum(0), ySum(0);
  int n = points.size();
  for (int i = 0; i < n; i++) {
    xSum += points[i].getX();
    ySum += points[i].getY();
  }
  return Point(xSum / n, ySum / n);
}

Point average(const vector<Point> & points, const vector<double> & weights)
{
  double xSum(0), ySum(0);
  double totalWeight = 0;
  for (unsigned int i = 0; i < points.size(); i++) {
    xSum += points[i].getX() * weights[i];
    ySum += points[i].getY() * weights[i];
    totalWeight += weights[i];
  }
  return Point(xSum, ySum) / totalWeight;
}

double stdDev(const vector<Point> & points)
{
  Point middle = average(points);
  double squareDist(0);
  int n = points.size();
  for (int i = 0; i < n; i++) {
    double dist = middle.getDist(points[i]);
    squareDist += dist * dist; 
  }
  return sqrt(squareDist / n);
}
