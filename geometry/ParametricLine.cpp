#include <cmath>
#include <iostream>

#include "ParametricLine.hpp"

#define EPSILON 0.000001

using namespace std;

double ParametricLine::getRho() const{
  return distanceToPoint(Point(0.0,0.0));
}

Angle ParametricLine::getTheta() const{
  if (getRho() == 0) {//Exception
    return direction.getTheta() + 90;
  }
  Point p = projectPoint(Point(0,0));
  return p.getTheta();
}

bool ParametricLine::isEquivalentTo(const ParametricLine & other) const{
  return isParallelTo(other) && distanceToPoint(other.origin) < EPSILON;
}

bool ParametricLine::isParallelTo(const ParametricLine & other) const{
  return direction.isCollinear(other.direction);
}

double ParametricLine::distanceToPoint(const Point & p) const{
  Point diff = origin - p;
  Angle deltaTheta = getDirection().getTheta() - diff.getTheta();
  return abs(sin(deltaTheta.getSignedValue() * M_PI / 180) * diff.getLength());
}

Point ParametricLine::projectPoint(const Point & p) const{
  Point diff = p - origin;
  return origin + direction * Point::dotProduct(diff,
                                                direction);
}

/* Coming from :
 * http://geomalgorithms.com/a05-_intersect-1.html#intersect2D_2Segments%28%29
 * The version presented here is simplified, because we won't compute between segment
 * but only between lines.
 */
Point ParametricLine::intersection(const ParametricLine & other) const{
  if (isParallelTo(other)){
    throw CollinearException();
  }
  Point diff = origin - other.origin;
  double denominator = Point::perpDotProduct(direction, other.direction);
  double numerator = Point::perpDotProduct(other.direction, diff);
  double ratio = numerator / denominator;
  return origin + direction * ratio;
}

ParametricLine ParametricLine::average(const ParametricLine & l1,
                                       const ParametricLine & l2)
{
  return weightedAverage(l1, l2, 1.0, 1.0);
}
ParametricLine ParametricLine::weightedAverage(const ParametricLine & l1,
                                               const ParametricLine & l2,
                                               double weight1, double weight2)
{
  Point inter;
  try{
    inter = l1.intersection(l2);
  }
  catch(CollinearException e) {
    inter = (l1.origin * weight1 + l2.origin * weight2) / (weight1 + weight2);
  }
  Point dir;
  Angle deltaTheta = l1.getDirection().getTheta() - l2.getDirection().getTheta();
  if (abs(deltaTheta.getSignedValue()) <= 90.0) {
    dir = l1.getDirection() * weight1 + l2.getDirection() * weight2;
  }
  else {
    dir = l1.getDirection() * weight1 - l2.getDirection() * weight2;
  }
  return ParametricLine(inter, inter + dir);
}

ostream& operator<<(ostream& out, const ParametricLine& p){
  return out << "[rho :" << p.getRho() << ", theta : " << p.getTheta() << "]";
}

Angle ParametricLine::angleDiff(const ParametricLine & l1,
                                const ParametricLine & l2)
{
  Angle a1 = l1.getDirection().getTheta();
  Angle a2 = l2.getDirection().getTheta();
  Angle diff = a1 - a2;
  //If absolute angle is greater than a quarter, then a smaller angle exists
  if (abs(diff.getSignedValue()) > 90.0) {
    diff = diff + Angle(180.0);
  }
  return diff;
}

ParametricLine ParametricLine::fromRhoTheta(double rho, const Angle & theta)
{
  Point p1(rho * cos(theta), rho * sin(theta));
  Point dir = Point::mkPointFromPolar(1.0, theta + Angle(90));
  return ParametricLine(p1, p1 + dir);
}

// rhoTol : pixels
// thetaTol : degrees
bool similarLines(const ParametricLine & l1, const ParametricLine & l2,
                  double rhoTol, double thetaTol)
{
  bool rhoOk = abs(l1.getRho() - l2.getRho()) < rhoTol;
  bool thetaOk = abs(ParametricLine::angleDiff(l1,l2).getSignedValue()) < thetaTol;
  return rhoOk && thetaOk;
}

vector<ParametricLine> mergeLines(const vector<ParametricLine> & lines,
                                  double rhoTol, double thetaTol)
{
  std::vector<unsigned int> scores;
  return mergeLines(lines, rhoTol, thetaTol, scores);
}

vector<ParametricLine> mergeLines(const vector<ParametricLine> & lines,
                                  double rhoTol, double thetaTol,
                                  vector<unsigned int> & scores)
{
  vector<ParametricLine> mergedLines;
  scores.clear();
  for (unsigned int i = 0; i < lines.size(); i++) {
    ParametricLine newLine = lines[i];
    bool merged = false;
    for (unsigned int j = 0; j < mergedLines.size(); j++) {
      ParametricLine oldLine = mergedLines[j];
      if (similarLines(newLine, oldLine, rhoTol, thetaTol)) {
        double score = scores[j];
        ParametricLine result = ParametricLine::weightedAverage(newLine,
                                                                oldLine,
                                                                1, score);
        mergedLines[j] = result;
        scores[j]++;
        merged = true;
      }
    }
    if (!merged) {
      mergedLines.push_back(newLine);
      scores.push_back(1);
    }
  }
  return mergedLines;
}
