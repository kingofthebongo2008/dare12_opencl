#pragma once

#include <tuple>

#include <opencl/opencl_context.h>
#include <opencl/opencl_command_queue.h>

#include "freeform_common.h"
#include "freeform_context.h"

#include "opencl/opencl_freeform_split_eval.h"

#include <iostream>


namespace freeform
{
    inline patches split( const freeform::context* ctx, const patches& p, float pixel_size)
    {
        bolt::cl::device_vector<uint32_t> element_count(ctx->get_control());
        bolt::cl::device_vector<uint32_t> keys(ctx->get_control());

        patches n(ctx->get_control() );
        
        n.resize(p.size() * 2 );
        keys.resize(p.size() * 2);
        element_count.resize(1);

        using namespace opencl;

        auto program    = create_freeform_split_eval_kernel( ctx->get_context() );
        auto kernel     = program->create_kernel("kernel_main");

        kernel->set_argument(0, p.getBuffer() );
        kernel->set_argument(1, n.getBuffer() );
        kernel->set_argument(2, keys.getBuffer() );
        kernel->set_argument(3, element_count.getBuffer());
        kernel->set_argument(4, pixel_size * 64.0f );

        ctx->launch1d( kernel.get(), 5 );
        ctx->synchronize();

        uint32_t c = element_count[0];

        std::cout << c << std::endl;

        return std::move(n);
    }
}
