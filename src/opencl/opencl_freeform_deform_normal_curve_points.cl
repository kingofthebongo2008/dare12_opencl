#include "opencl_stdint.h"

#include "opencl_math_functions.h"
#include "opencl_freeform_patch.h"

float4 make_x(freeform_patch p)
{
    return set(p.x0, p.x1, p.x2, p.x3);
}

float4 make_y(freeform_patch p)
{
    return set(p.y0, p.y1, p.y2, p.y3);
}

inline float norm_l2(float x, float y)
{
    return  sqrt(x * x + y * y);
}

//calculate normal vector of q cubic bezier poin, assumes in s are the tangent (derivative) values of the bezier curve
freeform_sample normal_vector(freeform_sample s)
{
    float d0 = norm_l2(s.x0, s.y0);
    float d1 = norm_l2(s.x1, s.y1);
    float d2 = norm_l2(s.x2, s.y2);
    float d3 = norm_l2(s.x3, s.y3);


    float mul0 = 1.0f / d0;
    float mul1 = 1.0f / d1;
    float mul2 = 1.0f / d2;
    float mul3 = 1.0f / d3;


    //float mul0 = d0 < 0.0001f ? 0.0f: 1.0f / d0;
    //float mul1 = d1 < 0.0001f ? 0.0f: 1.0f / d1;
    //float mul2 = d2 < 0.0001f ? 0.0f: 1.0f / d2;
    //float mul3 = d3 < 0.0001f ? 0.0f: 1.0f / d3;


    //convert to union tangent vector
    float x0 = s.x0 * mul0;
    float x1 = s.x1 * mul1;
    float x2 = s.x2 * mul2;
    float x3 = s.x3 * mul3;

    float y0 = s.y0 * mul0;
    float y1 = s.y1 * mul1;
    float y2 = s.y2 * mul2;
    float y3 = s.y3 * mul3;

    //obtain normal components as a rotation in the plane of the tangent components of 90 degrees
    float n_x_0 = y0;
    float n_y_0 = -x0;

    float n_x_1 = y1;
    float n_y_1 = -x1;

    float n_x_2 = y2;
    float n_y_2 = -x2;

    float n_x_3 = y3;
    float n_y_3 = -x3;

    freeform_sample r;

    r.x0 = n_x_0;
    r.x1 = n_x_1;
    r.x2 = n_x_2;
    r.x3 = n_x_3;

    r.y0 = n_y_0;
    r.y1 = n_y_1;
    r.y2 = n_y_2;
    r.y3 = n_y_3;

    return r;

}

freeform_patch compute_derivatives( freeform_patch p)
{
    float4 x = make_x(p);
    float4 y = make_y(p);

    //compute the control points of the derivative curve
    float4 dx = math_cubic_bezier_derivative(x);
    float4 dy = math_cubic_bezier_derivative(y);

    freeform_patch r;

    r.x0 = dx.x;// math::get_x(dx);
    r.x1 = dx.y;// math::get_y(dx);
    r.x2 = dx.z;// math::get_z(dx);
    r.x3 = dx.w;// math::get_w(dx);

    r.y0 = dy.x;// math::get_x(dy);
    r.y1 = dy.y;// math::get_y(dy);
    r.y2 = dy.z;// math::get_z(dy);
    r.y3 = dy.w;// math::get_w(dy);

    return r;
}


kernel void kernel_main(const global read_only freeform_patch* patches, global write_only freeform_sample* normals)
{
    uint32_t tid = get_global_id(0);

    freeform_patch p = patches[tid];

    float4 t = set(0.0f, 1.0f / 3.0f, 2.0f / 3.0f, 3.0f / 3.0f);

    freeform_patch  derivative = compute_derivatives(p);

    freeform_sample s = multi_eval_patch_3(p, t);             //sample the bezier curve
    freeform_sample s_derivative = multi_eval_patch_2(derivative, t);    //sample the derivative

    freeform_sample normal = normal_vector(s_derivative);

    normals[tid] = normal;
}



