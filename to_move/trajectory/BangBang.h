#pragma once

#include <Eigen/Core>

/// Implement bang bang law for multi-dimensional space with a limit on
/// the norm of speed and acceleration vectors
class BangBang
{
public:
  BangBang(double max_vel, double max_acc);

  /// Return the time required to reach dst from src,
  /// In this case, initial speed and final speed are 0
  double getTime(const Eigen::VectorXd & src,
                 const Eigen::VectorXd & dst) const;

  /// Return the target position after 'elapsed_time' on the trajectory from
  /// 'src' to 'dst' (0 speed at src and dst)
  Eigen::VectorXd getPosition(const Eigen::VectorXd & src,
                              const Eigen::VectorXd & dst,
                              double elapsed_time) const;

  /// Limit on the norm of acceleration vector
  double max_vel;
  /// Limit on the norm of speed vector
  double max_acc;

private:
  /// Time required to reach full speed from zero speed
  double accelerationTime() const;

  /// Distance required to reach full speed from zero speed
  double accelerationDistance() const;

  /// Distance traveled in the given time with max acceleration
  double accelerationDistance(double time) const;

  /// Is the system able to reach full speed from src to dst
  /// In this case, initial speed and final speed are 0
  bool reachFullSpeed(const Eigen::VectorXd & src,
                      const Eigen::VectorXd & dst) const;
};
