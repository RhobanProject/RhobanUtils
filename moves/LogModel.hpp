#pragma once

#include <Types/MatrixLabel.hpp>
#include <Types/VectorLabel.hpp>
#include <Utils/Scheduling.hpp>
#include <Model/HumanoidModel.hpp>
#include <Model/HumanoidFixedModel.hpp>

/**
 * Simple utility class logging
 * Lowlevel 
 */
class LogModel
{
    public:

        /**
         * Initialization
         */
        LogModel();

        /**
         * Start stop and cancel
         * logging
         * Logs are writen into given path
         * (with leading '/')
         */
        void start();
        void stop(const std::string& path);
        void cancel();

        /**
         * Return true if logging is ongoing
         */
        bool isLogging() const;

        /**
         * Append to log all servos
         * position and goal, sensors,
         * and model base state
         */
        void logServosPos();
        void logServosGoal();
        void logServos();
        void logSensors();
        void logPressure();
        void logBase(const Leph::HumanoidModel& model);
        void logModel(Leph::HumanoidFixedModel& model);

        /**
         * Add other data to log
         */
        void logData(const Leph::VectorLabel vect);
        void logData(const std::string& name, double value);

        /**
         * Append current data line to
         * log container and timestamp it
         */
        void flush();

        /**
         * Direct access to data vector and logged
         * data container
         */
        const Leph::VectorLabel& data() const;
        const Leph::MatrixLabel& container() const;

        /**
         * Return current timestamp in milliseconds
         */
        double time() const;

    private:
        
        /**
         * True if logging is ongoing
         */
        bool _isLogging;

        /**
         * Current logging line
         */
        Leph::VectorLabel _data;

        /**
         * Logs lines container
         */
        Leph::MatrixLabel _container;

        /**
         * Hold timestamp
         */
        Leph::Scheduling _scheduling;
};

