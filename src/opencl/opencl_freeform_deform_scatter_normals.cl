#include "opencl_stdint.h"

#include "opencl_math_functions.h"
#include "opencl_freeform_patch.h"



kernel void kernel_main(const global read_only freeform_sample* samples, global write_only freeform_point* points)
{
    uint32_t tid = get_global_id(0);

    freeform_sample p = samples[tid];

    freeform_point p0;
    freeform_point p1;
    freeform_point p2;
    freeform_point p3;

    p0.x = p.x0;
    p1.x = p.x1;
    p2.x = p.x2;
    p3.x = p.x3;

    p0.y = p.y0;
    p1.y = p.y1;
    p2.y = p.y2;
    p3.y = p.y3;


    points[4 * tid + 0] = p0;
    points[4 * tid + 1] = p1;
    points[4 * tid + 2] = p2;
    points[4 * tid + 3] = p3;
}



