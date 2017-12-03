#ifndef INTERPOLATION_HPP
#define INTERPOLATION_HPP

#include <list>
#include <vector>

#include "ParametricLine.hpp"

/* Compute an interpolation of the line defined by the tuple x,y given by
 * using the least square method.
 */
ParametricLine leastSquareLine(std::vector<double> x, std::vector<double> y);

/* returns coef */
std::pair<unsigned long long, double> leastSquareLine(std::list<unsigned long long> x, std::list<unsigned long long> y);

#endif//INTERPOLATION_HPP
