#pragma once

#include <tuple>

#include <opencl/opencl_context.h>
#include <opencl/opencl_command_queue.h>

#include "freeform_common.h"
#include "freeform_context.h"

#include "opencl/opencl_freeform_generate_sample.h"
#include "opencl/opencl_freeform_generate_struct.h"


namespace freeform
{
    class generate_sample : public ::generate_sample
    {
        public:
        generate_sample(float center_x, float center_y, float radius, float step) 
        {
            m_center_x  = center_x;
            m_center_y  = center_y;
            m_radius    = radius;
            m_step      = step;
        }
    };

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

        generate_sample params(static_cast<float> (center_image_x), static_cast<float> (center_image_y), radius, pas_pt_patch);

        using namespace opencl;

        auto program    = create_freeform_generate_sample_kernel( ctx->get_context() );
        auto kernel     = program->create_kernel("kernel_main");

        kernel->set_argument(0, n.getBuffer() );
        kernel->set_argument(1, np.getBuffer() );
        kernel->set_argument(2, params );

        ctx->launch1d( kernel.get(), iterations / 3 );
        ctx->synchronize();

        return std::move(std::make_tuple(std::move(n), std::move(np)));
    }
}
