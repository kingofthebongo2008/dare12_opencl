#include "opencl_stdint.h"

#include "opencl_radix_sort.h"

kernel void kernel_main(global write_only uint32_t * output, const global read_only uint* input, global write_only uint* summary, int stride) 
{
    int gidx = get_global_id(0);
    int gidy = get_global_id(1);
    int Index = gidy * stride +gidx;
    output[Index] = 0;
     
    if(gidx > 0)
    {
        for(int i =0;i<gidx;i++)
            output[Index] += input[gidy * stride +i];
    }
        
    if ( gidx == stride - 1 )
    {
        summary[gidy] = output[Index] + input[gidy * stride + (stride - 1)];
    }
} 




