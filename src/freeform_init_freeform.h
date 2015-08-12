#pragma once

#include <tuple>

#include <opencl/opencl_context.h>
#include <opencl/opencl_command_queue.h>

#include "freeform_common.h"
#include "freeform_context.h"


namespace freeform
{
    inline std::tuple<samples, patches> initialize_freeform( const freeform::context* ctx, float center_image_x, float center_image_y, float radius, uint32_t patch_count)
    {
        samples n(ctx->get_control() );
        patches np ( ctx->get_control() );

        auto pi = 3.1415926535f;
        auto pas = 2 * pi / patch_count;
        auto pas_pt_patch = pas / 3.0f;

        auto iterations = static_cast<uint32_t> (ceilf(2 * pi / pas_pt_patch));

        n.resize(iterations / 3);
        np.resize(iterations / 3);



        return std::move(std::make_tuple(std::move(n), std::move(np)));
    }
}
