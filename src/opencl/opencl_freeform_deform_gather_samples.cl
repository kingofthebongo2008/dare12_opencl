#include "opencl_stdint.h"

#include "opencl_math_functions.h"
#include "opencl_freeform_patch.h"



kernel void kernel_main(const global read_only freeform_point* points, const global read_only freeform_patch* patches, global write_only freeform_patch* deformed_patches)
{
    uint32_t tid = get_global_id(0);

    freeform_point r0 = points[tid + 0];
    freeform_point r1 = points[tid + 1];
    freeform_point r2 = points[tid + 2];
    freeform_point r3 = points[tid + 3];

    freeform_point p0 = points[tid + 4];
    freeform_point p1 = points[tid + 5];
    freeform_point p2 = points[tid + 6];
    freeform_point p3 = points[tid + 7];

    freeform_sample  s;

    //form delta of moved points
    s.x0 = r0.x - p0.x;
    s.x1 = r1.x - p1.x;
    s.x2 = r2.x - p2.x;
    s.x3 = r3.x - p3.x;

    s.y0 = r0.y - p0.y;
    s.y1 = r1.y - p1.y;
    s.y2 = r2.y - p2.y;
    s.y3 = r3.y - p3.y;


    //obtain delta of moved control points
    freeform_patch r = interpolate_curve(s);

    freeform_patch p = patches[tid];

    freeform_patch res;

    res.x0 = p.x0 + r.x0;
    res.x1 = p.x1 + r.x1;
    res.x2 = p.x2 + r.x2;
    res.x3 = p.x3 + r.x3;

    res.y0 = p.y0 + r.y0;
    res.y1 = p.y1 + r.y1;
    res.y2 = p.y2 + r.y2;
    res.y3 = p.y3 + r.y3;

    deformed_patches[tid] = res;

}



