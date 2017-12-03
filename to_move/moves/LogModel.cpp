#include "LogModel.hpp"

// XXX: Update with RhAL!

LogModel::LogModel() :
    _isLogging(false),
    _data(),
    _container(),
    _scheduling()
{
}

void LogModel::start()
{
    _isLogging = true;
    _data = Leph::VectorLabel();
    _container.clear();
    _scheduling = Leph::Scheduling();
}
void LogModel::stop(const std::string& path)
{
    if (_isLogging) {
        _isLogging = false;
        std::string filename = std::string(path + "model_") 
            + Leph::currentDate() + std::string(".log");
        _container.save(filename);
    } 
}
void LogModel::cancel()
{
    _isLogging = false;
}
        
bool LogModel::isLogging() const
{
    return _isLogging;
}

void LogModel::logServosPos()
{
    if (!_isLogging) {
        return;
    }

    /*
    _data.setOrAppend("pos:left_ankle_roll", 
        DEG2RAD(lowlevel.servos.get("left_ankle_roll")->currentAngle));
    _data.setOrAppend("pos:left_ankle_pitch", 
        DEG2RAD(lowlevel.servos.get("left_ankle_pitch")->currentAngle));
    _data.setOrAppend("pos:left_knee", 
        DEG2RAD(lowlevel.servos.get("left_knee")->currentAngle));
    _data.setOrAppend("pos:left_hip_pitch", 
        DEG2RAD(lowlevel.servos.get("left_hip_pitch")->currentAngle));
    _data.setOrAppend("pos:left_hip_roll", 
        DEG2RAD(lowlevel.servos.get("left_hip_roll")->currentAngle));
    _data.setOrAppend("pos:left_hip_yaw", 
        DEG2RAD(lowlevel.servos.get("left_hip_yaw")->currentAngle));
    _data.setOrAppend("pos:right_ankle_roll", 
        DEG2RAD(lowlevel.servos.get("right_ankle_roll")->currentAngle));
    _data.setOrAppend("pos:right_ankle_pitch", 
        DEG2RAD(lowlevel.servos.get("right_ankle_pitch")->currentAngle));
    _data.setOrAppend("pos:right_knee", 
        DEG2RAD(lowlevel.servos.get("right_knee")->currentAngle));
    _data.setOrAppend("pos:right_hip_pitch", 
        DEG2RAD(lowlevel.servos.get("right_hip_pitch")->currentAngle));
    _data.setOrAppend("pos:right_hip_roll", 
        DEG2RAD(lowlevel.servos.get("right_hip_roll")->currentAngle));
    _data.setOrAppend("pos:right_hip_yaw", 
        DEG2RAD(lowlevel.servos.get("right_hip_yaw")->currentAngle));
    _data.setOrAppend("pos:left_shoulder_pitch", 
        DEG2RAD(lowlevel.servos.get("left_shoulder_pitch")->currentAngle));
    _data.setOrAppend("pos:left_shoulder_roll", 
        DEG2RAD(lowlevel.servos.get("left_shoulder_roll")->currentAngle));
    _data.setOrAppend("pos:left_elbow", 
        DEG2RAD(lowlevel.servos.get("left_elbow")->currentAngle));
    _data.setOrAppend("pos:right_shoulder_pitch", 
        DEG2RAD(lowlevel.servos.get("right_shoulder_pitch")->currentAngle));
    _data.setOrAppend("pos:right_shoulder_roll", 
        DEG2RAD(lowlevel.servos.get("right_shoulder_roll")->currentAngle));
    _data.setOrAppend("pos:right_elbow", 
        DEG2RAD(lowlevel.servos.get("right_elbow")->currentAngle));
    _data.setOrAppend("pos:head_yaw", 
        DEG2RAD(lowlevel.servos.get("head_yaw")->currentAngle));
    _data.setOrAppend("pos:head_pitch", 
        DEG2RAD(lowlevel.servos.get("head_pitch")->currentAngle));
    
    _data.setOrAppend("pos_valid:left_ankle_roll", 
        lowlevel.servos.get("left_ankle_roll")->isLastUpdateValid);
    _data.setOrAppend("pos_valid:left_ankle_pitch", 
        lowlevel.servos.get("left_ankle_pitch")->isLastUpdateValid);
    _data.setOrAppend("pos_valid:left_knee", 
        lowlevel.servos.get("left_knee")->isLastUpdateValid);
    _data.setOrAppend("pos_valid:left_hip_pitch", 
        lowlevel.servos.get("left_hip_pitch")->isLastUpdateValid);
    _data.setOrAppend("pos_valid:left_hip_roll", 
        lowlevel.servos.get("left_hip_roll")->isLastUpdateValid);
    _data.setOrAppend("pos_valid:left_hip_yaw", 
        lowlevel.servos.get("left_hip_yaw")->isLastUpdateValid);
    _data.setOrAppend("pos_valid:right_ankle_roll", 
        lowlevel.servos.get("right_ankle_roll")->isLastUpdateValid);
    _data.setOrAppend("pos_valid:right_ankle_pitch", 
        lowlevel.servos.get("right_ankle_pitch")->isLastUpdateValid);
    _data.setOrAppend("pos_valid:right_knee", 
        lowlevel.servos.get("right_knee")->isLastUpdateValid);
    _data.setOrAppend("pos_valid:right_hip_pitch", 
        lowlevel.servos.get("right_hip_pitch")->isLastUpdateValid);
    _data.setOrAppend("pos_valid:right_hip_roll", 
        lowlevel.servos.get("right_hip_roll")->isLastUpdateValid);
    _data.setOrAppend("pos_valid:right_hip_yaw", 
        lowlevel.servos.get("right_hip_yaw")->isLastUpdateValid);
    _data.setOrAppend("pos_valid:left_shoulder_pitch", 
        lowlevel.servos.get("left_shoulder_pitch")->isLastUpdateValid);
    _data.setOrAppend("pos_valid:left_shoulder_roll", 
        lowlevel.servos.get("left_shoulder_roll")->isLastUpdateValid);
    _data.setOrAppend("pos_valid:left_elbow", 
        lowlevel.servos.get("left_elbow")->isLastUpdateValid);
    _data.setOrAppend("pos_valid:right_shoulder_pitch", 
        lowlevel.servos.get("right_shoulder_pitch")->isLastUpdateValid);
    _data.setOrAppend("pos_valid:right_shoulder_roll", 
        lowlevel.servos.get("right_shoulder_roll")->isLastUpdateValid);
    _data.setOrAppend("pos_valid:right_elbow", 
        lowlevel.servos.get("right_elbow")->isLastUpdateValid);
    _data.setOrAppend("pos_valid:head_yaw", 
        lowlevel.servos.get("head_yaw")->isLastUpdateValid);
    _data.setOrAppend("pos_valid:head_pitch", 
        lowlevel.servos.get("head_pitch")->isLastUpdateValid);
    */
}
void LogModel::logServosGoal()
{
    if (!_isLogging) {
        return;
    }

    /*
    _data.setOrAppend("goal:left_ankle_roll", 
        DEG2RAD(lowlevel.servos.get("left_ankle_roll")->goalAngle));
    _data.setOrAppend("goal:left_ankle_pitch", 
        DEG2RAD(lowlevel.servos.get("left_ankle_pitch")->goalAngle));
    _data.setOrAppend("goal:left_knee", 
        DEG2RAD(lowlevel.servos.get("left_knee")->goalAngle));
    _data.setOrAppend("goal:left_hip_pitch", 
        DEG2RAD(lowlevel.servos.get("left_hip_pitch")->goalAngle));
    _data.setOrAppend("goal:left_hip_roll", 
        DEG2RAD(lowlevel.servos.get("left_hip_roll")->goalAngle));
    _data.setOrAppend("goal:left_hip_yaw", 
        DEG2RAD(lowlevel.servos.get("left_hip_yaw")->goalAngle));
    _data.setOrAppend("goal:right_ankle_roll", 
        DEG2RAD(lowlevel.servos.get("right_ankle_roll")->goalAngle));
    _data.setOrAppend("goal:right_ankle_pitch", 
        DEG2RAD(lowlevel.servos.get("right_ankle_pitch")->goalAngle));
    _data.setOrAppend("goal:right_knee", 
        DEG2RAD(lowlevel.servos.get("right_knee")->goalAngle));
    _data.setOrAppend("goal:right_hip_pitch", 
        DEG2RAD(lowlevel.servos.get("right_hip_pitch")->goalAngle));
    _data.setOrAppend("goal:right_hip_roll", 
        DEG2RAD(lowlevel.servos.get("right_hip_roll")->goalAngle));
    _data.setOrAppend("goal:right_hip_yaw", 
        DEG2RAD(lowlevel.servos.get("right_hip_yaw")->goalAngle));
    _data.setOrAppend("goal:left_shoulder_pitch", 
        DEG2RAD(lowlevel.servos.get("left_shoulder_pitch")->goalAngle));
    _data.setOrAppend("goal:left_shoulder_roll", 
        DEG2RAD(lowlevel.servos.get("left_shoulder_roll")->goalAngle));
    _data.setOrAppend("goal:left_elbow", 
        DEG2RAD(lowlevel.servos.get("left_elbow")->goalAngle));
    _data.setOrAppend("goal:right_shoulder_pitch", 
        DEG2RAD(lowlevel.servos.get("right_shoulder_pitch")->goalAngle));
    _data.setOrAppend("goal:right_shoulder_roll", 
        DEG2RAD(lowlevel.servos.get("right_shoulder_roll")->goalAngle));
    _data.setOrAppend("goal:right_elbow", 
        DEG2RAD(lowlevel.servos.get("right_elbow")->goalAngle));
    _data.setOrAppend("goal:head_yaw", 
        DEG2RAD(lowlevel.servos.get("head_yaw")->goalAngle));
    _data.setOrAppend("goal:head_pitch", 
        DEG2RAD(lowlevel.servos.get("head_pitch")->goalAngle));
    */
}
void LogModel::logServos()
{
    logServosPos();
    logServosGoal();
}
void LogModel::logSensors()
{
    if (!_isLogging) {
        return;
    }

    /*
    _data.setOrAppend("sensor:pitch", 
        DEG2RAD(lowlevel.sensors.get("Pitch")->value));
    _data.setOrAppend("sensor:roll", 
        -DEG2RAD(lowlevel.sensors.get("Roll")->value));
    _data.setOrAppend("sensor:acc_x", 
        lowlevel.sensors.get("AccX")->value);
    _data.setOrAppend("sensor:acc_y", 
        lowlevel.sensors.get("AccY")->value);
    _data.setOrAppend("sensor:gyro_x", 
        DEG2RAD(lowlevel.sensors.get("GyroX")->value));
    _data.setOrAppend("sensor:gyro_y", 
        DEG2RAD(lowlevel.sensors.get("GyroY")->value));
    _data.setOrAppend("sensor:gyro_z", 
        DEG2RAD(lowlevel.sensors.get("GyroZ")->value));
    _data.setOrAppend("sensor:gyro_yaw", 
        DEG2RAD(lowlevel.sensors.get("GyroYaw")->value));
    */
}
void LogModel::logPressure()
{
    if (!_isLogging) {
        return;
    }

    /*
#ifdef ROBOT_SIGMABAN
    _data.setOrAppend("pressure:weight", 
        lowlevel.pressure.weight/1000.0);
    _data.setOrAppend("pressure:left_ratio", 
        lowlevel.pressure.leftRatio);
    _data.setOrAppend("pressure:right_ratio", 
        lowlevel.pressure.rightRatio);
    _data.setOrAppend("pressure:left_x", 
        lowlevel.pressure.leftX/100.0);
    _data.setOrAppend("pressure:right_x", 
        lowlevel.pressure.rightX/100.0);
    _data.setOrAppend("pressure:left_y", 
        lowlevel.pressure.leftY/100.0);
    _data.setOrAppend("pressure:right_y", 
        lowlevel.pressure.rightY/100.0);
#endif
#ifdef ROBOT_GROSBAN
    _data.setOrAppend("pressure:weight", 
        lowlevel.pressure.weight);
    _data.setOrAppend("pressure:left_ratio", 
        lowlevel.pressure.leftRatio);
    _data.setOrAppend("pressure:right_ratio", 
        lowlevel.pressure.rightRatio);
    _data.setOrAppend("pressure:left_x", 
        lowlevel.pressure.leftX);
    _data.setOrAppend("pressure:right_x", 
        lowlevel.pressure.rightX);
    _data.setOrAppend("pressure:left_y", 
        lowlevel.pressure.leftY);
    _data.setOrAppend("pressure:right_y", 
        lowlevel.pressure.rightY);
#endif
*/
}
void LogModel::logBase(const Leph::HumanoidModel& model)
{
    if (!_isLogging) {
        return;
    }

    _data.setOrAppend("pos:base_x", model.getDOF("base_x"));
    _data.setOrAppend("pos:base_y", model.getDOF("base_y"));
    _data.setOrAppend("pos:base_z", model.getDOF("base_z"));
    _data.setOrAppend("pos:base_yaw", model.getDOF("base_yaw"));
    _data.setOrAppend("pos:base_pitch", model.getDOF("base_pitch"));
    _data.setOrAppend("pos:base_roll", model.getDOF("base_roll"));
}
void LogModel::logModel(Leph::HumanoidFixedModel& model)
{
    if (!_isLogging) {
        return;
    }

    Eigen::Vector3d trunk = model.get().position("trunk", "origin");
    Eigen::Vector3d leftFoot = model.get().position("left_foot_tip", "origin");
    Eigen::Vector3d rightFoot = model.get().position("right_foot_tip", "origin");
    Eigen::Vector3d deltaLeftFoot = model.get()
        .position("left_foot_tip", "right_foot_tip");
    Eigen::Vector3d deltaRightFoot = model.get()
        .position("right_foot_tip", "left_foot_tip");
    _data.setOrAppend("model:trunk_x", trunk.x());
    _data.setOrAppend("model:trunk_y", trunk.y());
    _data.setOrAppend("model:trunk_z", trunk.z());
    _data.setOrAppend("model:left_foot_x", leftFoot.x());
    _data.setOrAppend("model:left_foot_y", leftFoot.y());
    _data.setOrAppend("model:left_foot_z", leftFoot.z());
    _data.setOrAppend("model:right_foot_x", rightFoot.x());
    _data.setOrAppend("model:right_foot_y", rightFoot.y());
    _data.setOrAppend("model:right_foot_z", rightFoot.z());
    _data.setOrAppend("model:is_support_left", 
        (model.getSupportFoot() == Leph::HumanoidFixedModel::LeftSupportFoot));
    _data.setOrAppend("model:left_step_x", deltaLeftFoot.x());
    _data.setOrAppend("model:left_step_y", deltaLeftFoot.y());
    _data.setOrAppend("model:left_step_z", deltaLeftFoot.z());
    _data.setOrAppend("model:right_step_x", deltaRightFoot.x());
    _data.setOrAppend("model:right_step_y", deltaRightFoot.y());
    _data.setOrAppend("model:right_step_z", deltaRightFoot.z());
}
        
void LogModel::logData(const Leph::VectorLabel vect)
{
    if (!_isLogging) {
        return;
    }

    _data.mergeUnion(vect);
}
void LogModel::logData(const std::string& name, double value)
{
    if (!_isLogging) {
        return;
    }

    _data.setOrAppend(name, value);
}

void LogModel::flush()
{
    if (!_isLogging) {
        return;
    }

    _data.setOrAppend("time:timestamp", _scheduling.timestamp());
    _container.append(_data);
}
        
const Leph::VectorLabel& LogModel::data() const
{
    return _data;
}
const Leph::MatrixLabel& LogModel::container() const
{
    return _container;
}
        
double LogModel::time() const
{
    return _scheduling.timestamp();
}

