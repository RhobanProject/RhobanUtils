#include "BangBang.h"

BangBang::BangBang(double max_vel_, double max_acc_)
  : max_vel(max_vel_), max_acc(max_acc_)
{
}

double BangBang::getTime(const Eigen::VectorXd & src,
                         const Eigen::VectorXd & dst) const
{
  double dist = (dst-src).norm();
  if (reachFullSpeed(src, dst)) {
    double dist_at_full_speed = dist - 2 * accelerationDistance();
    double time_at_full_speed = dist_at_full_speed / max_vel;
    return 2 * accelerationTime() + time_at_full_speed;
  }
  // Warning: dist = 2 * d (acceleration + decceleration)
  double acc_time = std::sqrt(dist / max_acc);//d = 1/2 a t^2 -> t = sqrt(2 d / a)
  return 2 * acc_time;
}

Eigen::VectorXd BangBang::getPosition(const Eigen::VectorXd & src,
                                      const Eigen::VectorXd & dst,
                                      double elapsed_time) const
{
  double traveled_dist = 0;
  double total_dist = (dst-src).norm();
  // What are the acceleration time / remaining time
  double acc_time = accelerationTime();
  if (!reachFullSpeed(src, dst))
  {
    acc_time = std::sqrt(total_dist / max_acc);//d = 1/2 a t^2 -> t = sqrt(2 d / a)
  }
  double total_time = getTime(src, dst);
  double remaining_time = total_time - elapsed_time;
  // If we have arrived, return dst
  if (remaining_time < 0) return dst;
  // Acceleration phase
  if (elapsed_time < acc_time)
  {
    traveled_dist = accelerationDistance(elapsed_time);
  }
  // Deceleration phase
  else if (remaining_time < acc_time)
  {
    traveled_dist = total_dist - accelerationDistance(remaining_time);
  }
  // Full speed phase
  else {
    double full_speed_time = elapsed_time - acc_time;
    traveled_dist = accelerationDistance() + full_speed_time * max_vel;
  }
  return src + traveled_dist / total_dist * (dst - src);
}

double BangBang::accelerationTime() const
{
  return max_vel / max_acc;
}

double BangBang::accelerationDistance() const
{
  double t = accelerationTime();
  return accelerationDistance(t);
}

double BangBang::accelerationDistance(double time) const
{
  return 1.0 / 2.0 * max_acc * time * time;// 1/2 alpha t^2
}


bool BangBang::reachFullSpeed(const Eigen::VectorXd & src,
                              const Eigen::VectorXd & dst) const
{
  double dist = (src - dst).norm();
  return dist > 2 * accelerationDistance();
}
