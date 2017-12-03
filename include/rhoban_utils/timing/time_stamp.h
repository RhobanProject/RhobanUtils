#pragma once

#include <chrono>

namespace rhoban_utils {

class TimeStamp : public std::chrono::time_point<std::chrono::steady_clock> 
{
    public:

        TimeStamp();
        TimeStamp(const std::chrono::time_point<std::chrono::steady_clock> & timePoint);
        static TimeStamp now();

        static TimeStamp fromMS(unsigned long msSinceEpoch);

        double getTimeMS() const;

};

}

double diffSec(const rhoban_utils::TimeStamp & src,
    const rhoban_utils::TimeStamp & dst);
double diffMs(const rhoban_utils::TimeStamp & src,
    const rhoban_utils::TimeStamp & dst);

bool operator<(const rhoban_utils::TimeStamp & ts1,
    const rhoban_utils::TimeStamp & ts2);
bool operator>(const rhoban_utils::TimeStamp & ts1,
    const rhoban_utils::TimeStamp & ts2);
