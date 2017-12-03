#ifndef PARAMETRIC_LINE_HPP
#define PARAMETRIC_LINE_HPP

#include <exception>

#include "Angle.hpp"
#include "Point.hpp"

class CollinearException : public std::exception {
  const char * what() const throw() { return "Collinear objects unexpected\n";};
};

class ParametricLine {
private:
  Point origin;
  Point direction;

public:
  ParametricLine(): origin(), direction(1.0, 0.0) {};
  ParametricLine(const ParametricLine & other): origin(other.origin), 
                                                direction(other.direction) {};
  ParametricLine(const Point & p1,
                 const Point & p2): origin(p1){
    direction = p2 - p1;
    direction.normalize(1.0);
  };

  //Warning! As calculated here, rho can only be positive, this result with
  //         the problem that two different lines can have the same rho and
  //         theta. e.g. y = 1 and y = -1
  double getRho() const;
  Angle getTheta() const;

  /* return a normalized vector of size 1 in one of the two directions of the
   * line.
   */
  const Point & getDirection() const { return direction;};

  double distanceToPoint(const Point & p) const;

  Point projectPoint(const Point & p) const;

  bool isEquivalentTo(const ParametricLine & other) const;
  bool isParallelTo(const ParametricLine & other) const;

/* Return the intersection of the line with another line
 * Throw CollinearException if Lines are parallels
 */
  Point intersection(const ParametricLine & other) const;

  // The averages passes through the intersection of the two lines, weight
  // determines the direction. If lines are parallel, weights determine the
  // origin of the average line
  static ParametricLine average(const ParametricLine & l1,
                                const ParametricLine & l2);
  static ParametricLine weightedAverage(const ParametricLine & l1,
                                        const ParametricLine & l2,
                                        double weight1, double weight2);

  // Return the smallest angle diff between two lines (-90 to 90)
  static Angle angleDiff(const ParametricLine & l1,
                         const ParametricLine & l2);

  static ParametricLine fromRhoTheta(double rho, const Angle & theta);
};

std::ostream& operator<<(std::ostream& out, const ParametricLine& p);

// thetaTol: degrees
bool similarLines(const ParametricLine & l1, const ParametricLine & l2,
                  double rhoTol, double thetaTol);

/* Merge all the lines in lines if they are similars enough according to
 * rhoTol and thetaTol given. The number of lines used for each line is set in
 * score. (result[i] has used scores[i] lines.
 */
std::vector<ParametricLine> mergeLines(const std::vector<ParametricLine> & lines,
                                       double rhoTol, double thetaTol);
std::vector<ParametricLine> mergeLines(const std::vector<ParametricLine> & lines,
                                       double rhoTol, double thetaTol,
                                       std::vector<unsigned int> & scores);
#endif//PARAMETRIC_LINE_HPP
