#include "rhoban_utils/control/variation_bound.h"

namespace rhoban_utils
{

void VariationBound::update(double &output, double target, double limit, double dt)
{
    double maxVariation = limit*dt;
    double delta = target-output;

    if (delta > maxVariation) {
        delta = maxVariation;
    } else if (delta < -maxVariation) {
        delta = -maxVariation;
    }

    output += delta;
}

void VariationBound::update(float &output, float target, float limit, float dt)
{
    float maxVariation = limit*dt;
    float delta = target-output;

    if (delta > maxVariation) {
        delta = maxVariation;
    } else if (delta < -maxVariation) {
        delta = -maxVariation;
    }

    output += delta;
}

}
