#include <cmath>
#include <iostream>

#include <stdexcept>

#include "Interpolation.hpp"

using namespace std;

/* Source for algorithm :
 * - http://easycalculation.com/analytical/learn-least-square-regression.php
 */
ParametricLine leastSquareLine(vector<double> x, vector<double> y)
{
  // Line will be interpolated with y = a + bx
  int n = x.size();
  vector<double> xy;
  vector<double> x2;
  double sum_x(0), sum_y(0), sum_xy(0), sum_x2(0);
  for (int i = 0; i < n; i++) {
    xy.push_back(x[i] * y[i]);
    x2.push_back(x[i] * x[i]);
    sum_x += x[i];
    sum_y += y[i];
    sum_xy += x[i] * y[i];
    sum_x2 += x[i] * x[i];
  }
  // Special case : line should be vertical
  if (n * sum_x2 == sum_x * sum_x) {
    double x = sqrt(sum_x2);
    return ParametricLine(Point(x,0), Point(x,1));
  }
  double b = (n * sum_xy - sum_x * sum_y) / (n * sum_x2 - sum_x * sum_x);
  double a = (sum_y - b * sum_x) / n;
  return ParametricLine(Point(0,a), Point(1, a + b));
}

std::pair<unsigned long long, double> leastSquareLine(std::list<unsigned long long> x, std::list<unsigned long long> y)
{
	std::pair<unsigned long long, double> result(0,0);
	if (x.size() != y.size())
		throw std::runtime_error("x and y should have same size)");
	// Line will be interpolated with y = a + bx
	if (x.size() == 0 || y.size() == 0)
		return result;

	unsigned long long minx = x.front();
	unsigned long long miny = y.front();

	auto xit = x.begin();
	auto yit = y.begin();
	for (; xit != x.end(); xit++, yit++)
	{
		minx = (*xit < minx) ? *xit : minx;
		miny = (*yit < miny) ? *yit : miny;
	}

	int n = x.size();
	vector<unsigned long long> xy;
	vector<unsigned long long> x2;
	unsigned long long sum_x(0), sum_y(0), sum_xy(0), sum_x2(0);

	xit = x.begin();
	yit = y.begin();
	for (; xit != x.end(); xit++, yit++)
	{
		xy.push_back( (*xit - minx) * (*yit - miny) );
		x2.push_back( (*xit - minx) * (*xit - minx) );
		sum_x += *xit;
		sum_y += *yit;
		sum_xy += *xit * *yit;
		sum_x2 += *xit * *xit;
	}
	// Special case : line should be vertical
	if (n * sum_x2 == sum_x * sum_x) {
		//double x = sqrt(sum_x2);//LH: Commented because line wasn't used
		return result;
	}
//	cout << n * sum_xy - sum_x * sum_y - (n * sum_x2 - sum_x * sum_x) << " " << n * sum_x2 - sum_x * sum_x << endl;
//	double b = (n * sum_xy - sum_x * sum_y) / (n * sum_x2 - sum_x * sum_x);
//	double a = (sum_y - b * sum_x) / n;

	result.second = (1.0 * ((long long int) (1.0 * (n * sum_xy - sum_x * sum_y) - (long long int) (n * sum_x2 - sum_x * sum_x)))) / (n * sum_x2 - sum_x * sum_x);
	result.first = ((sum_y + miny) - (1.0 + result.second) * (sum_x + minx)) / n;
	return result;
}
