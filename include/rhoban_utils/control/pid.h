//*****************************************************************************
//
// File Name	: 'PID.h'
// Author	: Steve NGUYEN
// Contact      : steve.nguyen@labri.fr
// Created	: mercredi, juin  3 2015
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//
// Notes:	notes
//
//*****************************************************************************

#pragma once

#include <cfloat>
#include <math.h>

namespace rhoban_utils
{

class PID
{
public:

    PID();
    PID(float k_p, float k_i, float k_d);
    float compute(float setpoint, float feedback, float elapsed);

    // bool inverted;

    float K_P;
    float K_I;
    float K_D;

    float output_max;
    float output_min;

    float p_max;
    float p_min;

    float i_max;
    float i_min;

    float d_max;
    float d_min;

    float P_val;
    float I_val;
    float D_val;


    float current_setpoint;
    float current_pos;
    float current_time;

    float current_error;
    float prev_error;

    float total;
};

}
