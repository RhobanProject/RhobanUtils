#pragma once

class Control
{
    public:
        Control();
        void update(float value);

        // Minimum and maximum for the output
        float min, max;
        // Gain of the proportional 
        float k_p;
        // Output of the controller
        float output;
};
