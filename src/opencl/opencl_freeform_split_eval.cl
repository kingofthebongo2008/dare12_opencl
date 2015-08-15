#include "opencl_stdint.h"

#include "opencl_math_functions.h"
#include "opencl_freeform_patch.h"

kernel void kernel_main(const global read_only freeform_patch* patches_in, global write_only freeform_patch* patches_out, global write_only uint32_t* keys, global read_write uint32_t* element_count, const read_only float seuil)
{
    uint32_t key = get_global_id(0);

    freeform_patch p = patches_in[key];

    float    d_0 = math_distance(p.x0, p.y0, p.x1, p.y1);
    float    d_1 = math_distance(p.x1, p.y1, p.x2, p.y2);
    float    d_2 = math_distance(p.x2, p.y2, p.x3, p.y3);

    float    m = max(max(d_0, d_1), d_2);


    if ( m > seuil )
    {
        float4 g1u = set(0.0f, 1.0f / 6.0f, 2.0f / 6.0f, 3.0f / 6.0f);
        float4 g2u = set(3.0f / 6.0f, 4.0f / 6.0f, 5.0f / 6.0f, 6.0f / 6.0f);

        freeform_sample   g1 = multi_eval_patch_3(p, g1u);
        freeform_sample   g2 = multi_eval_patch_3(p, g2u);

        freeform_patch    g4 = interpolate_curve(g1);
        freeform_patch    g5 = interpolate_curve(g2);

        uint32_t old = atomic_add(element_count, 2);

        patches_out[old] = g4;
        patches_out[old + 1] = g5;

        //store the index number, so we can sort later to maintain the order
        keys[old] = key * 10 + 1;
        keys[old + 1] = key * 10 + 2;
    }
    else
    {
        uint32_t old = atomic_add(element_count, 1);

        patches_out[old]    = p;
        keys[old]           = key * 10;
    }


    
}



