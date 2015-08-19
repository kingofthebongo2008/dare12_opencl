#include "opencl_stdint.h"

#include "opencl_math_functions.h"
#include "opencl_freeform_patch.h"

kernel void kernel_main(const global read_only freeform_patch* patches, global write_only freeform_point* points)
{
    uint32_t tid = get_global_id(0);

    freeform_patch p    = patches[tid];

    float4 t          = set(0.0f, 1.0f / 3.0f, 2.0f / 3.0f, 3.0f / 3.0f);
    freeform_sample s = multi_eval_patch_3(p, t);             //sample the bezier curve


    freeform_point p0;
    freeform_point p1;
    freeform_point p2;
    freeform_point p3;

    p0.x = s.x0;
    p1.x = s.x1;
    p2.x = s.x2;
    p3.x = s.x3;

    p0.y = s.y0;
    p1.y = s.y1;
    p2.y = s.y2;
    p3.y = s.y3;

    points[4 * tid + 0] = p0;
    points[4 * tid + 1] = p1;
    points[4 * tid + 2] = p2;
    points[4 * tid + 3] = p3;
}



