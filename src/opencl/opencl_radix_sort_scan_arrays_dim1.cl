#include "opencl_stdint.h"

#include "opencl_radix_sort.h"

 kernel void kernel_main(global write_only uint *output, const global read_only uint32_t *input, local read_write uint* block, const uint block_size ) 
{
    int tid = get_local_id(0);
    int gid = get_global_id(0);
    int bid = get_group_id(0);
    
    /* Cache the computational window in shared memory */
    block[tid]     = input[gid];

    uint cache = block[0];

    /* build the sum in place up the tree */
    for(int stride = 1; stride < block_size; stride *=2)
    {
        
        if(tid>=stride)
        {
            cache = block[tid-stride]+block[tid];
        }
        barrier(CLK_LOCAL_MEM_FENCE);

        block[tid] = cache;
        barrier(CLK_LOCAL_MEM_FENCE);
        
    }
        
    /*write the results back to global memory */
    if(tid == 0)
    {	
        output[gid]     = 0;
    }
    else
    {
        output[gid]     = block[tid-1];
    }
} 

