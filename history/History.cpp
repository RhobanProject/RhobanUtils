#include <geometry/Angle.hpp>
#include "history/History.h"

History::History(double window) : 
    _mutex(),
    _isLogging(false),
    _startLoggingTime(-1.0),
    _windowSize(window),
    _values()
{
}

void History::setWindowSize(double window)
{
    _windowSize = window;
}

size_t History::size() const
{
    return _values.size();
}

const std::pair<double, double>& History::front() const
{
    if (_values.size() == 0) {
        static std::pair<double,double> zero = {0.0, 0.0};
        return zero;
    } else {
        return _values.front();
    }
}
const std::pair<double, double>& History::back() const
{
    if (_values.size() == 0) {
        static std::pair<double,double> zero = {0.0, 0.0};
        return zero;
    } else {
        return _values.back();
    }
}

void History::pushValue(double timestamp, double value)
{
    //Lock
    _mutex.lock();
    //Check that timestamp is increasing
    if (_values.size() > 0 && timestamp < _values.back().first) {
        _mutex.unlock();
        throw std::logic_error("History invalid timestamp");
    }
    if (_values.size() > 0 && timestamp == _values.back().first) {
        _mutex.unlock();
        return;
    }
    //Insert the value
    _values.push_back(std::pair<double, double>(timestamp, value));
    //Shrink the queue if not in logging mode
    while (
        !_isLogging &&
        !_values.empty() && 
        (_values.back().first - _values.front().first > _windowSize)
    ) {
        _values.pop_front();
    }
    //Set the startLoggingTime to the first
    //data timestampt pushed after startLogging() is called
    if (_isLogging && _startLoggingTime < 0.0) {
        _startLoggingTime = timestamp;
    }
    //Unlock
    _mutex.unlock();
}

double History::interpolate(double timestamp, History::ValueType valueType) const
{
    //Lock
    _mutex.lock();

    //Degererate failback cases
    if (_values.size() == 0) {
        _mutex.unlock();
        return 0.0;
    } else if (_values.size() == 1) {
        _mutex.unlock();
        return _values.front().second;
    } else if (timestamp <= _values.front().first) {
        _mutex.unlock();
        return _values.front().second;
    } else if (timestamp >= _values.back().first) {
        _mutex.unlock();
        return _values.back().second;
    }

    //Bijection search
    size_t indexLow = 0;
    size_t indexUp = _values.size() - 1;
    while (indexUp - indexLow > 1) {
        size_t indexMiddle = (indexLow + indexUp)/2;
        if (_values[indexMiddle].first <= timestamp) {
            indexLow = indexMiddle;
        } else {
            indexUp = indexMiddle;
        }
    }

    //Retrieve lower and upper bound values
    double tsLow = _values[indexLow].first;
    double valLow = _values[indexLow].second;
    double tsUp = _values[indexUp].first;
    double valUp = _values[indexUp].second;

    //Unlock
    _mutex.unlock();
    
    //Weights
    double wLow = (tsUp-timestamp)/(tsUp-tsLow);
    double wUp = (timestamp-tsLow)/(tsUp-tsLow);

    if (valueType == Number) {
        //Return linear interpolated value
        return wLow*valLow + wUp*valUp;
    } else if (valueType == AngleRad) {
        Angle angleLow(rad2deg(valLow));
        Angle angleUp(rad2deg(valUp));
        auto result = Angle::weightedAverage(angleLow, wLow, angleUp, wUp);

        return deg2rad(result.getSignedValue());
    } else {
        throw std::logic_error("History unknown value type for interpolate");
    }
}

void History::startLogging()
{
    _mutex.lock();
    _isLogging = true;
    _startLoggingTime = -1.0;
    _mutex.unlock();
}

void History::stopLogging(std::ostream& os, bool binary)
{
    _mutex.lock();
    _isLogging = false;
    if (!binary) {
        for (const auto& it : _values) {
            //Skip data in buffer before logging start
            if (
                _startLoggingTime > 0.0 && 
                it.first >= _startLoggingTime
            ) {
                //Write ascii data
                os 
                    <<  std::setprecision(15) << it.first << " " 
                    <<  std::setprecision(15) << it.second << std::endl;
            }
        }
    } else {
        // When writing in binary, use all values
        size_t size = _values.size();
        os.write((const char*)&size, sizeof(size_t));
        for (const auto& it : _values) {
            //Write binary data
            os.write((const char*)&(it.first), sizeof(double));
            os.write((const char*)&(it.second), sizeof(double));
        }
    }
    _mutex.unlock();
}

void History::loadReplay(
    std::ifstream& is, bool binary, double timeShift)
{
    _mutex.lock();
    //Clean the container
    _values.clear();
    //Read the number of data
    size_t size = 0;
    if (binary) {
        is.read((char*)&size, sizeof(size_t));
    }
    //Read the input stream
    while (true) {
        //Extract one data point
        double timestamp;
        double value;
        if (binary) {
            if (size == 0) {
                _mutex.unlock();
                return;
            }
            is.read((char*)&timestamp, sizeof(double));
            is.read((char*)&value, sizeof(double));
            size--;
        } else {
            while (is.peek() == ' ' || is.peek() == '\n') {
                is.ignore();
            }
            if (is.peek() == '#' || is.peek() == EOF) {
                _mutex.unlock();
                return;
            }
            is >> timestamp >> value;
        }
        //Apply time shift
        timestamp += timeShift;
        //Check that timestamp is increasing
        if (_values.size() > 0 && timestamp <= _values.back().first) {
           _mutex.unlock();
           throw std::runtime_error("History invalid timestamp");
        }
        //Insert the value
        _values.push_back(std::pair<double, double>(timestamp, value));
    }
}

