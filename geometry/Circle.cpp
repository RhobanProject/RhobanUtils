#include <cmath>

#include "Circle.hpp"

#include "ParametricLine.hpp"

#define EPSILON 0.000001

using namespace std;

void Circle::setCenter(const Point _center) {
    center = _center;
}

void Circle::setRadius(const double _radius) {
    radius = _radius;
}

Angle Circle::getTheta(const Point & p) const{
  Point diff = p - center;
  return diff.getTheta();
}

Point Circle::getPoint(Angle theta) const{
  return center + Point::mkPointFromPolar(radius, theta);
}
  
bool Circle::contains(const Point &p) const
{
    return (p-center).getLength() <= radius;
}

void Circle::translate(const Point & delta)
{
  center = center + delta;
}

Circle Circle::circleFromTwoTangents(const Point & p1,
                                     const Point & d1,
                                     const Point & p2,
                                     const Point & d2){
  // Intersection of the two perpendiculars mark the center of the circle
  Point perpD1 = d1.perpendicular();
  Point perpD2 = d2.perpendicular();
  ParametricLine l1(p1, perpD1 + p1);
  ParametricLine l2(p2, perpD2 + p2);
  Point center;
  try{
    center = l1.intersection(l2);
  }
  catch (CollinearException e){
    if (!l1.isEquivalentTo(l2)){
      throw ImpossibleCircleException();
    }
    //We got the opposite sides of the circle
    center = (p1 + p2) / 2;
  }
  double radius = center.getDist(p1);
  if (radius - center.getDist(p2) > EPSILON){
    throw ImpossibleCircleException();
  }
  return Circle(center, radius);
}

Circle Circle::circleFromPoints(const Point & p1,
                                const Point & p2,
                                const Point & p3){
  float h2_x = (float)p1.getX() + 0.5*((float)p3.getX()-(float)p1.getX());
  float h2_y = (float)p1.getY() + 0.5*((float)p3.getY()-(float)p1.getY());
  float h3_x = (float)p1.getX() + 0.5*((float)p2.getX()-(float)p1.getX());
  float h3_y = (float)p1.getY() + 0.5*((float)p2.getY()-(float)p1.getY());
  float v2_x = (float)p1.getY() - (float)p3.getY();
  float v2_y = (float)p3.getX() - (float)p1.getX();
  float v3_x = (float)p1.getY() - (float)p2.getY();
  float v3_y = (float)p2.getX() - (float)p1.getX();
  float det= v2_x*v3_y - v2_y*v3_x;
  //No solution (3 points on a line)
  if (fabs(det) < EPSILON) {
    throw ImpossibleCircleException();
  }
  float beta = (v2_x*h2_y + v2_y*(h3_x-h2_x) - v2_x*h3_y)/det;
  Circle c;
  float c_x(h3_x + beta*v3_x);
  float c_y(h3_y + beta*v3_y);
  c.center = Point(c_x, c_y);
  c.radius = sqrt( (c_x-p1.getX())*(c_x-p1.getX())
                   +(c_y-p1.getY())*(c_y-p1.getY()));
  return c;
}

Circle Circle::inscribedCircle(const Point & p1,
                               const Point & p2,
                               const Point & p3)
{
  Point d12 = (p2 - p1).normalize();
  Point d13 = (p3 - p1).normalize();
  Point d21 = -d12;
  Point d23 = (p3 - p2).normalize();
  ParametricLine bissector1(p1, p1 + d12 + d13);
  ParametricLine bissector2(p2, p2 + d21 + d23);
  Point center;
  try{
    center = bissector1.intersection(bissector2);
  }
  catch(const CollinearException & exc) {
    throw ImpossibleCircleException();
  }
  double radius = ParametricLine(p1, p2).distanceToPoint(center);
  return Circle(center, radius);
}

Point Circle::projectPoint(const Point & p) const{
  Point diff = p - center;
  diff.normalize(radius);
  return center + diff;
}

std::vector<Point> Circle::tangents(const Point &p)
{
    std::vector<Point> result;
    Point vect = p - center;
    double dist = vect.getLength();

    if (dist > radius) {
        double alpha = acos(radius / dist);
        Angle rot(rad2deg(alpha));
        auto a = center + vect.rotation(rot).normalize(radius);
        auto b = center + vect.rotation(-rot).normalize(radius);

        result.push_back(a);
        result.push_back(b);
    }

    return result;
}

ostream & operator<<(ostream & out, const Circle & c){
  out << '{' << c.getRadius() << ',' << c.getCenter() << '}';
  return out;
}
  
