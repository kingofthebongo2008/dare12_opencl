#pragma once

namespace freeform
{
    namespace details
    {
        inline void create_shader_bezier_ds(voids)
        {
            //strange? see in the hlsl file
            static
            #include "opencl_grayscale.ir"
        }
    }
}


