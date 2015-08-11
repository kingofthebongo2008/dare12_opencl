#pragma once

#include "opencl_stdint.h"
float4 make_float4(float x, float y, float z, float w)
{
    return (float4)(x, y, z, w);
}

float4 zero()
{
    return make_float4(0.0f, 0.0f, 0.0f, 0.0f);
}

float4 one()
{
    return make_float4(1.0f, 1.0f, 1.0f, 1.0f);
}

float4 minus_one()
{
    return make_float4(-1.0f, -1.0f, -1.0f, -1.0f);
}

float4 identity_r0()
{
    return make_float4(1.0f, 0.0f, 0.0f, 0.0f);
}

float4 identity_r1()
{
    return make_float4(0.0f, 1.0f, 0.0f, 0.0f);
}

float4 identity_r2()
{
    return make_float4(0.0f, 0.0f, 1.0f, 0.0f);
}

float4 identity_r3()
{
    return make_float4(0.0f, 0.0f, 0.0f, 1.0f);
}

float4 set(float v1, float v2, float v3, float v4)
{
    return make_float4(v1, v2, v3, v4);
}

float4 splat(float value)
{
    return make_float4(value, value, value, value);
}

//simple math operations
float4 add(float4 v1, float4 v2)
{
    return v1 + v2;
}
float4 sub(float4 v1, float4 v2)
{
    return v1 - v2;
}

float4 mul(float4 v1, float4 v2)
{
    return v1 * v2;
}

float4 div(float4 v1, float4 v2)
{
    return v1 / v2;
}

float4 mul_1(float4 v, float f)
{
    float4 v1 = splat(f);
    return mul(v, v1);
}

float4 rcp(float4 v)
{
    float4 v1 = one();
    return div(v, v1);
}

