#include "HeadScan.h"

#include <cmath>
#include <stdexcept>

HeadScan::HeadScan()
  : dirty(true), fov_width(37.5), fov_height(50),
    max_pan(80), min_tilt(-5), max_tilt(95),
    min_overlap(5),
    ctrl_law(360, 360 * 10)//Max speed 1Hz, max acc: 0.2 s to reach full speed
{
}

int HeadScan::getNbRows() const
{
  double missing_tilt = (max_tilt - min_tilt) - fov_height;
  // If camera cover the required field, one row is enough
  if (missing_tilt < 0) return 1;
  // Otherwise, check number of rows required
  double row_max_gain = (fov_height) - min_overlap;
  int extra_rows = std::ceil(missing_tilt / row_max_gain);
  return extra_rows + 1;
}

int HeadScan::getNbCols() const
{
  double missing_pan = 2 * max_pan - fov_width;
  // If camera cover the required field, one col is enough
  if (missing_pan < 0) return 1;
  // Otherwise, check number of cols required
  double col_max_gain = (fov_width) - min_overlap;
  int extra_cols = std::ceil(missing_pan / col_max_gain);
  return extra_cols + 1;
}

void HeadScan::setFOV(double new_fov_width, double new_fov_height)
{
  if (fov_width == new_fov_width &&
      fov_height == new_fov_height)
  {
    return;
  }
  dirty = true;
  fov_width = new_fov_width;
  fov_height = new_fov_height;
}

void HeadScan::setMinTilt(double new_min_tilt)
{
  if (new_min_tilt == min_tilt) return;
  dirty = true;
  min_tilt = new_min_tilt;
}

void HeadScan::setMaxTilt(double new_max_tilt)
{
  if (new_max_tilt == max_tilt) return;
  dirty = true;
  max_tilt = new_max_tilt;
}

void HeadScan::setMaxPan(double new_max_pan)
{
  if (new_max_pan == max_pan) return;
  dirty = true;
  max_pan = new_max_pan;
}

void HeadScan::setMinOverlap(double new_min_overlap)
{
  if (new_min_overlap == min_overlap) return;
  dirty = true;
  min_overlap = new_min_overlap;
}

void HeadScan::setControlLaw(double max_speed, double max_acc)
{
  if (max_speed == ctrl_law.max_vel &&
      max_acc == ctrl_law.max_acc)
  {
    return;
  }
  dirty = true;
  ctrl_law.max_vel = max_speed;
  ctrl_law.max_acc = max_acc;
}

void HeadScan::synchronize()
{
  if (!dirty) return;
  updateControlPoints();
  updatePassageTimes();
}

void HeadScan::updateControlPoints()
{
  int nb_rows = getNbRows();
  int nb_cols = getNbCols();
  /// Compute tilt properties
  double tilt_start, tilt_end, tilt_offset;
  if (nb_rows == 1) {
    tilt_start = (min_tilt + max_tilt) / 2;
    tilt_end = tilt_start;
    tilt_offset = 0;
  }
  else {
    tilt_start  = min_tilt + fov_height / 2;
    tilt_end    = max_tilt - fov_height / 2;
    tilt_offset = (tilt_end - tilt_start) / (nb_rows - 1);
  }
  // Compute pan properties
  double pan_start, pan_end, pan_offset;
  if (nb_cols == 1) {
    pan_start  = 0;
    pan_end    = 0;
    pan_offset = 0;
  }
  else {
    pan_start  = -max_pan + fov_width / 2;
    pan_end    =  max_pan - fov_width / 2;
    pan_offset = (pan_end - pan_start) / (nb_cols - 1);
  }
  // Add control points
  pan_targets.clear();
  tilt_targets.clear();
  for (int row = 0; row < nb_rows; row++)
  {
    // Computing tilt row
    double tilt;
    if (row == 0) { tilt = tilt_start; }
    else { tilt = tilt_end - (row - 1) *  tilt_offset; }
    // Computing pan
    double row_pan_start = pan_start;
    double row_pan_end   = pan_end;
    // If row is superior or equal to two, end limit change
    if (row >= 2) row_pan_end -= pan_offset;
    // If row is odd, swap sides
    if (row % 2 == 1) {
      double tmp = row_pan_end;
      row_pan_end = row_pan_start;
      row_pan_start = tmp;
    }
    // Push start and end of the line
    pan_targets.push_back(row_pan_start);
    tilt_targets.push_back(tilt);
    pan_targets.push_back(row_pan_end);
    tilt_targets.push_back(tilt);
  }
  // If there is only one row, we reached an appropriate location to end
  if (nb_rows == 1) return;
  // Depending on if the number of row is odd or even, start of next cycle changes
  if (nb_rows % 2 == 0) {
    pan_targets.push_back(pan_start);
  }
  else {
    pan_targets.push_back(pan_end);
  }
  tilt_targets.push_back(tilt_start);
}

void HeadScan::updatePassageTimes()
{
  int nb_points = pan_targets.size();
  double elapsed_time = 0;
  passage_times.clear();
  passage_times.push_back(0);
  for (int i = 1; i < nb_points; i++)
  {
    PanTilt src = getCtrlPoint(i - 1);
    PanTilt dst = getCtrlPoint(i);
    elapsed_time += ctrl_law.getTime(src, dst);
    passage_times.push_back(elapsed_time);
  }
}

double HeadScan::getCycleDistance() const
{
  int nb_points = tilt_targets.size();
  double dist = 0;
  for (int i = 0; i < nb_points - 1; i++)
  {
    double d_tilt = tilt_targets[i] - tilt_targets[i+1];
    double d_pan  = pan_targets [i] - pan_targets [i+1];
    dist += std::sqrt(d_tilt * d_tilt + d_pan * d_pan);
  }
  return dist;
}

double HeadScan::getCycleDuration() const
{
  if (passage_times.size() == 0) return 0;
  return passage_times[passage_times.size() - 1];
}

const std::vector<double> & HeadScan::getPanTargets() const
{
  return pan_targets;
}

const std::vector<double> & HeadScan::getTiltTargets() const
{
  return tilt_targets;
}

HeadScan::PanTilt HeadScan::getTarget(double time) const
{
  double cycle_time = std::fmod(time, getCycleDuration());
  int start_idx = getCtrlPointIdx(cycle_time);
  double extra_time = cycle_time - passage_times[start_idx];
  // Getting end of trajectories
  PanTilt src = getCtrlPoint(start_idx);
  PanTilt end = getCtrlPoint(start_idx + 1);
  // Final positions
  PanTilt target = ctrl_law.getPosition(src, end, extra_time);
  // If cycle is odd and number of rows is odd, invert pan
  if (isOddCycle(time) && getNbRows() % 2 == 1) { target(0) *= -1; }
  return target;
}

int HeadScan::getCtrlPointIdx(double time) const
{
  time = std::fmod(time, getCycleDuration());
  for (size_t i = 1; i < passage_times.size(); i++)
  {
    if (time < passage_times[i]) return i-1;
  }
  throw std::logic_error("HeadScan:getPanTarget: Not supposed to happen");
}

HeadScan::PanTilt HeadScan::getCtrlPoint(int idx) const
{
  Eigen::Vector2d result;
  result << pan_targets[idx], tilt_targets[idx];
  return result;
}

bool HeadScan::isOddCycle(double time) const
{
  int cycle = std::floor(time / getCycleDuration());
  return cycle % 2 == 1;
}
