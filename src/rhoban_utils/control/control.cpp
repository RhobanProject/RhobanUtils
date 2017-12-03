#include "rhoban_utils/control/control.h"

namespace rhoban_utils
{

Control::Control()
{
    k_p = 1.0;
    min = -100.0;
    max = -100.0;
}

void Control::update(float value)
{
    // P gain
    float tmp = k_p*value;

    // Bounding
    if (tmp < min) tmp = min;
    if (tmp > max) tmp = max;

    output = tmp;
}

}
