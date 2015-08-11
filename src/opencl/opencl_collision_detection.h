#pragma once

#include "opencl_freeform_patch.h"

typedef struct _segment
{
    freeform_point a;
    freeform_point b;
} freeform_segment;

float area(freeform_point a, freeform_point b, freeform_point c)
{
    return ( b.x - a.x ) * ( c.y - a.y) - (c.x - a.x) * (b.y - a.y);
}

bool left(freeform_point a, freeform_point b, freeform_point c)
{
    return area(a, b, c) > 0.0f;
}

bool colinear(freeform_point a, freeform_point b, freeform_point c)
{
    //todo: epsilon
    return area(a, b, c) == 0.0f;
}

//exclusive or: returns true exactly one argument is true
bool xor(bool x, bool y)
{
    //return !x ^ !y;
    return x ^ y;
}

bool intersect_segments_3(freeform_point a, freeform_point b, freeform_point c, freeform_point d)
{
    uint32_t a0 = colinear(a, b, c) ? 1 : 0 ;
    uint32_t a1 = colinear(a, b, d) ? 1 : 0;
    uint32_t a2 = colinear(c, d, a) ? 1 : 0;
    uint32_t a3 = colinear(c, d, b) ? 1 : 0;

    uint32_t co = a0 + a1 + a2 + a3;

    if (co == 0 )
    {
        //c - > d are left of the freeform_segment ab or vice versa
        return xor( left(a, b, c), left(a, b, d) ) && xor(left(c, d, a), left(c, d, b));
    }
    else
    {
        return false;
    }
}

bool intersect_segments(freeform_segment a, freeform_segment b)
{
    return intersect_segments_3(a.a, a.b, b.a, b.b);
}

freeform_point make_point_0(freeform_patch a)
{
    freeform_point r;
    r = make_point(a.x0, a.y0);
    return r;
}

freeform_point make_point_1(freeform_patch a)
{
    freeform_point r;
    r = make_point(a.x1, a.y1);
    return r;
}

freeform_point make_point_2(freeform_patch a)
{
    freeform_point r;
    r = make_point(a.x2, a.y2);
    return r;
}

freeform_point make_point_3(freeform_patch a)
{
    freeform_point r;
    r = make_point(a.x3, a.y3);
    return r;
}

freeform_segment make_segment(freeform_point a, freeform_point b)
{
    freeform_segment s;
    s.a = a;
    s.b = b;
    return s;
}

bool intersect_patches(freeform_patch a, freeform_patch b)
{
    freeform_segment s0 = make_segment(make_point_0(a), make_point_1(a));
    freeform_segment s1 = make_segment(make_point_1(a), make_point_2(a));
    freeform_segment s2 = make_segment(make_point_2(a), make_point_3(a));

    freeform_segment d0 = make_segment(make_point_0(b), make_point_1(b));
    freeform_segment d1 = make_segment(make_point_1(b), make_point_2(b));
    freeform_segment d2 = make_segment(make_point_2(b), make_point_3(b));

    uint32_t a00 = intersect_segments(s0, d0) ? 1 : 0;
    uint32_t a01 = intersect_segments(s0, d1) ? 1 : 0;
    uint32_t a02 = intersect_segments(s0, d2) ? 1 : 0;

    uint32_t a10 = intersect_segments(s1, d0) ? 1 : 0;
    uint32_t a11 = intersect_segments(s1, d1) ? 1 : 0;
    uint32_t a12 = intersect_segments(s1, d2) ? 1 : 0;

    uint32_t a20 = intersect_segments(s2, d0) ? 1 : 0;
    uint32_t a21 = intersect_segments(s2, d1) ? 1 : 0;
    uint32_t a22 = intersect_segments(s2, d2) ? 1 : 0;

    uint32_t co = a00 + a01 + a02 + a10 + a11 + a12 + a20 + a21 + a22;

    return co != 0;
    
}

