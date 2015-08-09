#pragma once

#include "opencl_stdint.h"
#include "opencl_math_vector.h"


float math_distance( float x1, float y1, float x2, float y2 )
{
    //calcul la distance entre deux points.
    float x = (x2 - x1) * (x2 - x1);
    float y = (y2 - y1) * (y2 - y1);

    return sqrt( x + y );
}

float lerp(float a, float b, float t)
{
    return ( 1 - t ) * a  +  t * b;
}

//decasteljau algorithm for cubic bezier, for evaluating the curves
float math_decasteljau_3( float4 points, float t)
{
    float b0_0 = lerp( points.x, points.y, t);
    float b0_1 = lerp( points.y, points.z, t);
    float b0_2 = lerp( points.z, points.w, t);

    float b1_0 = lerp(b0_0, b0_1, t);
    float b1_1 = lerp(b0_1, b0_2, t);

    float b2_0 = lerp(b1_0, b1_1, t);

    return b2_0;
}

//decasteljau algorith for quadratic bezier, for evaluating the derivatives
float math_decasteljau_2(float4 points, float t)
{
    float b0_0 = points.x;
    float b0_1 = points.y;
    float b0_2 = points.z;

    float b1_0 = lerp(b0_0, b0_1, t);
    float b1_1 = lerp(b0_1, b0_2, t);

    float b2_0 = lerp(b1_0, b1_1, t);
        
    return b2_0;
}

//returns the control points of the quadratic curve, which is a derivative of a cubic curve
float4 math_cubic_bezier_derivative( float4 points )
{
    //make vector p1, p2, p3
    float4 a = points.yzww; 
    float4 m = set(3.0f, 3.0f, 3.0f, 0.0f);

    //make curve which is

    // 3 * ( p1 - p0), 3 * ( p2 - p1),3 * ( p3 - p2)
    float4 r = mul(m, sub(a, points));

    return r;
}

