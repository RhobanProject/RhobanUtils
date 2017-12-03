#pragma once

#include "particle_filter/Observation.hpp"

template<class T>
class BoundedScoreObservation : public Observation<T>
{
public:
    /// Return the minimal score reachable for the given observation
    virtual double getMinScore() const = 0;
};
