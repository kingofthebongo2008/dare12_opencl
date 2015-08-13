#include "opencl_stdint.h"

#include "opencl_freeform_patch.h"

kernel void kernel_main( global write_only freeform_patch* patches, global write_only uint32_t* keys, global read_write uint32_t* element_count, const read_only float seuil )
{
    uint32_t i = get_global_id(0);
}



