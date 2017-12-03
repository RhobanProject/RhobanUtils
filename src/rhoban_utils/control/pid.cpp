//*****************************************************************************
//
// File Name	: 'PID.cpp'
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


#include "rhoban_utils/control/pid.h"
#include <iostream>

namespace rhoban_utils
{

PID::PID()
{

    // inverted=false;
    K_P=0.0;
    K_I=0.0;
    K_D=0.0;

    output_max=100.0;
    output_min=-100.0;

    p_max=100.0;
    p_min=-100.0;

    i_max=100.0;
    i_min=-100.0;

    d_max=100.0;
    d_min=-100.0;


    current_setpoint=0.0;
    current_pos=0.0;
    current_time=0.0;
    current_error=0.0;
    prev_error=0.0;

    P_val=0.0;
    I_val=0.0;
    D_val=0.0;
    total=0.0;
}


PID::PID(float k_p, float k_i, float k_d): K_P(k_p), K_I(k_i), K_D(k_d)
{


    output_max=100.0;
    output_min=-100.0;

    p_max=100.0;
    p_min=-100.0;

    i_max=100.0;
    i_min=-100.0;

    d_max=100.0;
    d_min=-100.0;


    current_setpoint=0.0;
    current_pos=0.0;
    current_time=0.0;
    current_error=0.0;
    prev_error=0.0;

    P_val=0.0;
    I_val=0.0;
    D_val=0.0;
    total=0.0;

}

float PID::compute(float setpoint, float feedback, float elapsed)
{

    if(fabs(current_setpoint-setpoint)>10.0) //should be a param...
        I_val=0.0;

    current_setpoint=setpoint;
    current_pos=feedback;
    current_error=(current_setpoint-current_pos);


    P_val=K_P*current_error;

        //bounds
    if(P_val<0.0)
        P_val=(P_val<p_min? p_min: P_val);
    else
        P_val=(P_val>p_max? p_max: P_val);

    I_val+=(K_I*current_error)*elapsed;

        //bounds
    if(I_val<0.0)
        I_val=(I_val<i_min? p_min: I_val);
    else
        I_val=(I_val>i_max? p_max: I_val);


    if(elapsed>FLT_EPSILON)
        D_val=K_D*(current_error-prev_error)/elapsed;
    else
        D_val=0.0; //uh?

    prev_error=current_error;

            //bounds
    if(D_val<0.0)
        D_val=(D_val<d_min? d_min: D_val);
    else
        D_val=(D_val>d_max? d_max: D_val);


    total= P_val+I_val+D_val;

    std::cout<<"PID DEBUG: "<<K_I<<" "<<setpoint<<" "<<feedback<<" "<<elapsed<<" "<<P_val<<" "<<I_val<<" "<<D_val<<" "<<total<<std::endl;

    if(total>0)
        return (total<output_min ? output_min : total);
    else
        return (total>output_max ? output_max : total);
}

}
