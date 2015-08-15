#include "opencl_stdint.h"

#include "opencl_radix_sort.h"

/**
 * @brief   Calculates block-histogram bin whose bin size is 256
 * @param   unsortedData    array of unsorted elements
 * @param   buckets         histogram buckets    
 * @param   shiftCount      shift count
 * @param   sharedArray     shared array for thread-histogram bins
  */
kernel void kernel_main( const global read_only uint32_t*  unsortedData, global write_only uint32_t*       buckets, uint32_t shiftCount, local uint32_t* sharedArray )
{
    size_t localId = get_local_id(0);
    size_t globalId = get_global_id(0);
    size_t groupId = get_group_id(0);
    size_t groupSize = get_local_size(0);
    
    uint numGroups = get_global_size(0) / get_local_size(0);
   
    /* Initialize shared array to zero */
    
        sharedArray[localId] = 0;

    barrier(CLK_LOCAL_MEM_FENCE);
    
    /* Calculate thread-histograms */
      uint value = unsortedData[globalId];
        value = (value >> shiftCount) & 0xFFU;
        atomic_inc(sharedArray+value);
    
    
    barrier(CLK_LOCAL_MEM_FENCE);
    
    /* Copy calculated histogram bin to global memory */
    
        uint bucketPos = groupId  * groupSize + localId ;
        //uint bucketPos = localId * numGroups + groupId ;
        buckets[bucketPos] = sharedArray[localId];
    
}





