#pragma once

#include "timing/TimeStamp.hpp"
#include "particle_filter/BoundedScoreObservation.hpp"

#include <cmath>
#include <deque>
#include <vector>

// Previous declaration
namespace Vision {
    namespace Utils {
        class CameraState;
    }
}

template<class T>
class PFWatcher {
public:
    class Entry {
    public:

        Entry() : ts(), consistency(0) {}
        Entry(const Utils::Timing::TimeStamp & ts_, double consistency_)
            : ts(ts_), consistency(consistency_)
            {}

        Utils::Timing::TimeStamp ts;
        double consistency;
    };

    typedef std::vector<BoundedScoreObservation<T> *> ObservationSet;

    PFWatcher() : windowSize(20), windowTime(15) {}

    /// Push the observations and remove outdated values
    void pushObservations(const Utils::Timing::TimeStamp & ts,
                          const ObservationSet & observations,
                          const T & representativeParticle)
        {
            for (const BoundedScoreObservation<T> * obs : observations) {
                double score = obs->potential(representativeParticle);
                // Currently, uses only binary scores
                double binaryScore = score > obs->getMinScore() ? 1 : 0;
                entries.push_back(Entry(ts, binaryScore));
            }
            removeExcess();
            removeOutdated(ts);
        }

    /// retrieve the consistency score in [0,1] (remove outdated information if necessary)
    double getScore(const Utils::Timing::TimeStamp & now)
        {
            removeOutdated(now);
            if (entries.size() == 0) {
                return 0;
            }

            double score = 0;
            for (const Entry e : entries) {
                score += e.consistency;
            }
            return score / entries.size();
        }

    void setWindowSize(int newWindowSize)
        {
            windowSize = newWindowSize;
        }

    void setWindowTime(int newWindowTime)
        {
            windowTime = newWindowTime;
        }

private:

    void removeExcess()
        {
            while ((int)entries.size() > windowSize) {
                entries.pop_front();
            }
        }

    void removeOutdated(const Utils::Timing::TimeStamp & now)
        {
            while (entries.size() > 0) {
                double elapsed = diffSec(entries.front().ts, now);
                if (elapsed < windowTime) break;
                entries.pop_front();
            }
        }

    /// entries inside the window
    std::deque<Entry> entries;
    /// Never store more than 'windowSize' observations
    int windowSize;
    /// Discard observations older than 'windowTime' [s]
    double windowTime;
};
