#pragma once

#include <tuple>

#include <opencl/opencl_context.h>
#include <opencl/opencl_command_queue.h>

#include "freeform_common.h"
#include "freeform_context.h"

#include "opencl/opencl_freeform_split_eval.h"


namespace freeform
{
    inline patches split( const freeform::context* ctx, const patches& p, float pixel_size)
    {
        patches n(ctx->get_control() );

        using namespace opencl;

        auto program    = create_freeform_split_eval_kernel( ctx->get_context() );
        auto kernel     = program->create_kernel("kernel_main");

        kernel->set_argument(0, p.getBuffer() );
        kernel->set_argument(1, n.getBuffer() );

        ctx->launch1d( kernel.get(), 5 );
        ctx->synchronize();

        return std::move(n);
    }
}
