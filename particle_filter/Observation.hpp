#pragma once

// An observation is only valid for the specified Particle class
template<class P>
class Observation {
public:

  virtual ~Observation() {};

  /* Return the potential of a a particle according to the observation. */
  virtual double potential(const P & p) const = 0;

  static double linearScore(double error,
                            double maxError,
                            double pFalsePositive)
    {
      double ratioError = error / maxError;
      double score = 1 - ratioError;
      score = score > 0 ? score : 0.0;
      score = score * (1 - pFalsePositive) + pFalsePositive;
      return score;
    }

  /**
   * Score based on (error / maxError)^2
   */
  static double quadraticScore(double error,
                               double maxError,
                               double pFalsePositive)
    {
      double ratioError = error / maxError;
      double score = 1 - ratioError;
      score = score > 0 ? score : 0.0;
      score = score * score;
      score = score * (1 - pFalsePositive) + pFalsePositive;
      return score;
    }

  /**
   * Score based on a sigmoid: 1 / (1 - e^x)
   * Sigmoid has been modified to ensure that the values start at 0 and end
   * at 1, therefore, score function s follows the following properties:
   * s''(offset) = 0
   * s'(offset) ~= lambda
   * s(offset) = 1 - offset / 2
   * s(0) = 0
   * s(1) = 1
   *
   * The value returned at end is :
   * pFalsePositive + (1 - pFalsePositive) * s(1 - error/maxError)
   */
  static double sigmoidScore(double error,
                             double maxError,
                             double pFalsePositive,
                             double offset,
                             double lambda)
    {
      double ratioError = error / maxError;
      double x, score;
      double vMin = 1 / (1 + exp(lambda * offset));
      double vMax = 1 / (1 + exp(-lambda * (1 - offset)));
      x = 1 - ratioError;
      x = x > 0 ? x : 0.0;
      score = (1 / (1 + exp(-lambda * (x - offset))));// score in [vMin,vMax]
      score = score - vMin + x * (1 + vMin - vMax);                     
      score = score * (1 - pFalsePositive) + pFalsePositive;
      return score;
    }

  virtual std::string toStr() const {
    return "[Unidentified observation]";
  }
};
