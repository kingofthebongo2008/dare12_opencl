#pragma once

#include "opencl_freeform_patch.h"

typedef struct _vector2
{
    float x;
    float y;
} vector2;

vector2 make_vector2(freeform_point a, freeform_point b)
{
    vector2 r;

    r.x = b.x - a.x;
    r.y = b.y - a.y;

    return r;
}

vector2 vector2_mul(float s, vector2 v)
{
    vector2 r;
    r.x = v.x * s;
    r.y = v.y * s;
    return r;
}

vector2 vector2_add(vector2 v1, vector2 v2)
{
    vector2 r;
    r.x = v1.x + v2.x;
    r.y = v1.y + v2.y;
    return r;
}

vector2 vector2_sub(vector2 v1, vector2 v2)
{
    vector2 r;
    r.x = v1.x + v2.x;
    r.y = v1.y + v2.y;
    return r;
}

float vector2_dot(vector2 v1, vector2 v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

float vector2_norm(vector2 v1)
{
    return sqrt(vector2_dot(v1, v1));
}

