#include "opencl_stdint.h"

#include "opencl_freeform_struct.h"
#include "opencl_freeform_generate.h"
#include "opencl_freeform_patch.h"


kernel void kernel_main( const read_only generate_sample gen, global write_only freeform_sample* samples, global write_only freeform_patch* patches)
{
    uint32_t i = get_global_id(0);

    freeform_sample s = sample(i, &gen);
    freeform_patch  p = interpolate_curve(s);

    samples[i] = s;
    patches[i] = p;
}



