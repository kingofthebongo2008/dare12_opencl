#include "opencl_stdint.h"

#include "opencl_math_functions.h"
#include "opencl_freeform_patch.h"

freeform_point  point_mul (float  v, freeform_point  p )
{
    freeform_point  r;

    r.x = p.x * v;
    r.y = p.y * v;

    return  r;
}

freeform_point  point_add(freeform_point a, freeform_point b)
{
    freeform_point  r;

    r.x = a.x * b.x;
    r.y = a.y * b.x;

    return  r;
}




kernel void kernel_main(const global read_only freeform_point* points_in, global write_only freeform_point* points_out, const read_only uint32_t count)
{
    uint32_t i = get_global_id(0);

    uint32_t i0 = 4 * i + 3;
    uint32_t i1 = 4 * i + 4;

    if (i1 == 4 * count)
    {
        i1 = 0;
    }

    freeform_point n0 = points_in[i0];
    freeform_point n1 = points_in[i1];

    freeform_point n = point_mul(0.5, point_add(n0, n1));

    points_out[i0] = n;
    points_out[i1] = n;
}



