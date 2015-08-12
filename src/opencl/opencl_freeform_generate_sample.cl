#include "opencl_stdint.h"

#include "opencl_freeform_struct.h"
#include "opencl_freeform_generate.h"
#include "opencl_freeform_patch.h"


kernel void kernel_main(global write_only freeform_sample* samples, global write_only freeform_patch* patches, const read_only generate_sample gen)
{
    uint32_t i = get_global_id(0);

    float x0 = sample_x(3 * i, 0, &gen);
    float x1 = sample_x(3 * i, 1, &gen);
    float x2 = sample_x(3 * i, 2, &gen);
    float x3 = sample_x(3 * i, 3, &gen);

    float y0 = sample_y(3 * i, 0, &gen);
    float y1 = sample_y(3 * i, 1, &gen);
    float y2 = sample_y(3 * i, 2, &gen);
    float y3 = sample_y(3 * i, 3, &gen);

    freeform_sample s = sample(i, &gen);

    freeform_patch  p = interpolate_curve(s);

    samples[i] = s;
    patches[i] = p;
}



