#include "opencl_stdint.h"

#include "opencl_radix_sort.h"

kernel void kernel_main( global write_only uint32_t* output, const global read_only uint *input, local uint* block, const uint block_size, const uint stride, global write_only uint* sumBuffer)
{

      int tidx = get_local_id(0);
      int tidy = get_local_id(1);
      int gidx = get_global_id(0);
      int gidy = get_global_id(1);
      int bidx = get_group_id(0);
      int bidy = get_group_id(1);
      
      int lIndex = tidy * block_size + tidx;
      int gpos = (gidx << RADIX) + gidy;
      int groupIndex = bidy * (get_global_size(0)/block_size) + bidx;
     
      
      /* Cache the computational window in shared memory */
      block[tidx] = input[gpos];
      barrier(CLK_LOCAL_MEM_FENCE);
      
      uint cache = block[0];

    /* build the sum in place up the tree */
    for(int dis = 1; dis < block_size; dis *=2)
    {
    
        if(tidx>=dis)
        {
            cache = block[tidx-dis]+block[tidx];
        }
        barrier(CLK_LOCAL_MEM_FENCE);

        block[tidx] = cache;
        barrier(CLK_LOCAL_MEM_FENCE);
    }

        
    /* store the value in sum buffer before making it to 0 */ 	
    sumBuffer[groupIndex] = block[block_size-1];

    /*write the results back to global memory */

    if(tidx == 0)
    {	
        
        output[gpos] = 0;
    }
    else
    {
        
        output[gpos] = block[tidx-1];
    }
}   




