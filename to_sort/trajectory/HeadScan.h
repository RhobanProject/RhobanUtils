#pragma once

#include "BangBang.h"

#include <vector>

/// HeadScan types:
/// For 2 rows
/// ------>----->-----|
/// |                 |
/// |-----<-----<------
/// For 3 rows
/// ------>----->-----|
///                  /|
/// ------>----->---/ |
/// |                 |
/// |-----<-----<------
/// For 4 rows
/// ------>----->-----|
/// |                 |
/// |-----<-----<---- |
///                 | |
/// ------>----->---| |
/// |                 |
/// |-----<-----<------
/// And so on...
/// 
/// When the number of rows is odd, the trajectories at odd cycles are vertical
/// mirrors of trajectories at even cycles
class HeadScan
{
public:
  typedef Eigen::Vector2d PanTilt;

  HeadScan();

  int getNbRows() const;
  int getNbCols() const;

  /// Update the field of view in both height and lateral
  void setFOV(double fov_width, double fov_height);
  void setMinTilt(double min_tilt);
  void setMaxTilt(double max_tilt);
  void setMaxPan(double max_pan);
  void setMinOverlap(double min_overlap);
  void setControlLaw(double max_speed, double max_acc);

  /// Update both control points and passage time if parameters have been modified
  void synchronize();

  void updateControlPoints();
  void updatePassageTimes();

  /// Return the total 'distance' of a cycle (in [deg])
  double getCycleDistance() const;
  double getCycleDuration() const;

  const std::vector<double> & getPanTargets() const;
  const std::vector<double> & getTiltTargets() const;

  PanTilt getTarget(double time) const;

private:
  /// Return the control point previous or equal to given time
  int getCtrlPointIdx(double time) const;

  /// Return the control point at the given idx
  PanTilt getCtrlPoint(int idx) const;

  /// Return true if time corresponds to an odd cycle
  bool isOddCycle(double time) const;

  /// False if parameters have been set but trajectory has not been updated
  bool dirty;

  /// Pan targets
  std::vector<double> pan_targets;
  /// Tilt targets
  std::vector<double> tilt_targets;
  /// Time point at which a control point is reached: control_point -> time_passage
  /// TimePoint are in [s]
  std::vector<double> passage_times;

  /// Field of view width [deg]
  double fov_width;
  /// Field of view height [deg]
  double fov_height;
  /// Maximal Pan in vision field of view in pan during scan is [-max_pan, max_pan] [deg]
  double max_pan;
  /// Min tilt [deg]
  double min_tilt;
  /// Max tilt [deg]
  double max_tilt;
  /// Required overlap to cover properly border of the image [deg]
  double min_overlap;

  /// Control law
  BangBang ctrl_law;
};
