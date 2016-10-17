#include "ElapseTick.h"
#include "TimeStamp.hpp"

ElapseTick::ElapseTick() :
    hasStats(false), 
    minTime(0.0),
    avgTime(0.0),
    maxTime(0.0),
    lastTimestamp(now())
{
}
        
ElapseTick::~ElapseTick()
{
}

void ElapseTick::tick()
{
    double nowT = now();
    double elapsed = (nowT-lastTimestamp)/1000.0;
    lastTimestamp = nowT;

    //Call tick implementation
    double start = now();
    bool isTicked = tick(elapsed);
    double stop = now();
    double duration = stop - start;

    //Compute stats
    if (isTicked) {
        if (!hasStats) {
            hasStats = true;
            minTime = duration;
            avgTime = duration;
            maxTime = duration;
        } else {
            if (duration < minTime) minTime = duration;
            if (duration > maxTime) maxTime = duration;
            avgTime = avgTime*0.99 + duration*0.01;
        }
    }
}
