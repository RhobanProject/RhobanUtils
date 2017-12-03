#include "rhoban_utils/timing/time_stamp.h"

using namespace std::chrono;

namespace rhoban_utils {

TimeStamp::TimeStamp(){}
TimeStamp::TimeStamp(const time_point<steady_clock> & timePoint)
    : time_point<steady_clock>(timePoint) {}

TimeStamp TimeStamp::now()
{
    return TimeStamp(steady_clock::now());
}

TimeStamp TimeStamp::fromMS(unsigned long msSinceEpoch)
{
    return TimeStamp(time_point<steady_clock>(
        milliseconds(msSinceEpoch)));
}

double TimeStamp::getTimeMS() const
{
    return duration_cast<std::chrono::duration<double, std::milli>>(
        time_since_epoch()).count();
}

}

double diffSec(const rhoban_utils::TimeStamp & src,
    const rhoban_utils::TimeStamp & dst)
{
    double elapsedTicks = (dst - src).count();
    return elapsedTicks * steady_clock::period::num / steady_clock::period::den;
}

double diffMs(const rhoban_utils::TimeStamp & src,
    const rhoban_utils::TimeStamp & dst)
{
    return diffSec(src, dst) * 1000;
}

bool operator<(const rhoban_utils::TimeStamp & ts1,
    const rhoban_utils::TimeStamp & ts2)
{
    return diffMs(ts1, ts2) > 0;
}
bool operator>(const rhoban_utils::TimeStamp & ts1,
    const rhoban_utils::TimeStamp & ts2)
{
    return diffMs(ts1, ts2) < 0;
}
