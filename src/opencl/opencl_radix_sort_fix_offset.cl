#include "opencl_stdint.h"

#include "opencl_radix_sort.h"

kernel void kernel_main( const global read_only uint32_t* input, global write_only uint32_t* output)
{

    int gidx = get_global_id(0);
    int gidy = get_global_id(1);
    int gpos = gidy + (gidx << RADIX);

    output[gpos] += input[gidy];
}


