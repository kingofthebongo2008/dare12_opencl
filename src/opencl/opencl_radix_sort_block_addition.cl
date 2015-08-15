#include "opencl_stdint.h"

#include "opencl_radix_sort.h"

kernel void kernel_main ( const global read_only uint32_t* input, global write_only uint32_t* output, uint32_t stride)
{
    
    int gidx = get_global_id(0);
    int gidy = get_global_id(1);
    int bidx = get_group_id(0);
    int bidy = get_group_id(1);
    
      
    int gpos = gidy + (gidx << RADIX);
     
    int groupIndex = bidy * stride + bidx;
      
    uint temp;
    temp = input[groupIndex];
      
    output[gpos] += temp;
}




