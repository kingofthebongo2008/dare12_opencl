#pragma once

#include "opencl_math_functions.h"
#include "opencl_math_matrix.h"

typedef struct _freeform_patch
{
    float x0;
    float x1;
    float x2;
    float x3;

    float y0;
    float y1;
    float y2;
    float y3;
} freeform_patch;

typedef struct _freeform_sample
{
    float x0;
    float x1;
    float x2;
    float x3;

    float y0;
    float y1;
    float y2;
    float y3;
} freeform_sample;

typedef struct _freeform_point
{
    float x;
    float y;
} freeform_point;

freeform_point make_point( float x, float y )
{
    freeform_point p = { x, y };
    return p;
}

//curve interpolation, find control points from curve points
freeform_patch interpolate_curve( freeform_sample p )
{
    //see equation (4) from the paper

    float4  v0 = identity_r0();
    float4  v1 = set(-5.0f / 6.0f, 3.0f, -3.0f / 2.0f, 1.0f / 3.0f);
    float4  v2 = v1.wzyx; //swizzle<w, z, y, x>(v1);
    float4  v3 = identity_r3();

    float4x4 m = matrix44_set(v0, v1, v2, v3);

    float4  p0_x = set(p.x0, p.x1, p.x2, p.x3);
    float4  p0_y = set(p.y0, p.y1, p.y2, p.y3);

    float4  x = matrix44_mul_r(m, p0_x);
    float4  y = matrix44_mul_r(m, p0_y);

    freeform_patch   r = {
                            x.x, x.y, x.z, x.w,
                            y.x, y.y, y.z, y.w
                         };

    return r;
}

//eval cubic bezier
freeform_point eval_patch_3( freeform_patch p, float t )
{
    float4  xs = set(p.x0, p.x1, p.x2, p.x3);
    float4  ys = set(p.y0, p.y1, p.y2, p.y3);

    float x = math_decasteljau_3(xs, t);
    float y = math_decasteljau_3(ys, t);
    return make_point(x, y);
}

//eval cubic bezier for 4 points
freeform_sample multi_eval_patch_3(freeform_patch p, float4 t)
{
    freeform_point r0 = eval_patch_3(p, t.x);
    freeform_point r1 = eval_patch_3(p, t.y);
    freeform_point r2 = eval_patch_3(p, t.z);
    freeform_point r3 = eval_patch_3(p, t.w);

    freeform_sample r = { r0.x, r1.x, r2.x, r3.x, r0.y, r1.y, r2.y, r3.y };

    return r;
}

//eval quadratic bezier
freeform_point eval_patch_2( freeform_patch p, float t )
{
    float4  xs = set(p.x0, p.x1, p.x2, 0.0f);
    float4  ys = set(p.y0, p.y1, p.y2, 0.0f);

    float   x = math_decasteljau_2(xs, t);
    float   y = math_decasteljau_2(ys, t);
    return make_point(x, y);
}

//eval quadratic bezier for 4 points
freeform_sample multi_eval_patch_2(freeform_patch p, float4 t)
{
    freeform_point r0 = eval_patch_2(p, t.x);
    freeform_point r1 = eval_patch_2(p, t.y);
    freeform_point r2 = eval_patch_2(p, t.z);
    freeform_point r3 = eval_patch_2(p, t.w);

    freeform_sample r = { r0.x, r1.x, r2.x, r3.x, r0.y, r1.y, r2.y, r3.y };

    return r;
}

float min4(float x0, float x1, float x2, float x3)
{
    float x = min(x0, x1);

    x = min(x, x2);
    x = min(x, x3);
    return x;
}

float max4(float x0, float x1, float x2, float x3)
{
    float x = max(x0, x1);

    x = max(x, x2);
    x = max(x, x3);
    return x;
}

bool intersect_bounding_boxes(float4 a, float4 b )
{
    //a and b contain: min_x, min_y, max_x, max_y for an aabb

    float x1 = a.x;// math::get_x(a);
    float y1 = a.y;// math::get_y(a);

    float x2 = a.z;// math::get_z(a);
    float y2 = a.w;// math::get_w(a);

    float x3 = b.x;// math::get_x(b);
    float y3 = b.y;// math::get_y(b);

    float x4 = b.z;// math::get_z(b);
    float y4 = b.z;// math::get_w(b);

    if (x2 < x3)
    {
        return false;
    }
        
    if ((x2 == x3) && (y2 < y3 || y4 < y1))
    {
        return false;
    }

    if (x4 < x1)
    {
        return false;
    }

    if ((x4 == x1) && (y4 < y1 || y2 < y3))
    {
        return false;
    }

    return true;
}

