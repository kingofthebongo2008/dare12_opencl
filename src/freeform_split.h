#pragma once

#include <tuple>

#include <bolt/cl/sort_by_key.h>

#include <opencl/opencl_context.h>
#include <opencl/opencl_command_queue.h>

#include "freeform_common.h"
#include "freeform_context.h"

#include "opencl/opencl_freeform_split_eval.h"

#include "freeform_print_utils.h"


namespace freeform
{
    inline patches split( freeform::context* ctx, const patches& p, float pixel_size)
    {
        bolt::cl::device_vector<uint32_t> element_count(ctx->get_control());
        bolt::cl::device_vector<uint32_t> keys(ctx->get_control());

        patches n(ctx->get_control() );
        
        auto s = p.size();

        n.resize(s * 2);
        keys.resize(s * 2);
        element_count.resize(1);

        using namespace opencl;

        auto program    = create_freeform_split_eval_kernel( ctx->get_context() );
        auto kernel     = program->create_kernel("kernel_main");

        kernel->set_argument(0, p.getBuffer() );
        kernel->set_argument(1, n.getBuffer() );
        kernel->set_argument(2, keys.getBuffer() );
        kernel->set_argument(3, element_count.getBuffer());
        kernel->set_argument(4, pixel_size * 64.0f );

        ctx->launch1d(kernel.get(), s);
        ctx->synchronize();

        auto new_size = element_count[0];

        n.resize(new_size);
        keys.resize(new_size);

        int keysa[8] = { 2, 9, 3, 7, 5, 6, 3, 8 };
        int valuesa[8] = { 100, 200, 16, 50, 15, 8, 3, 5 };
        
        //bolt::cl::sort_by_key(ctx->get_control(), keysa, keysa + 8, valuesa, bolt::cl::less<int>());


        ctx->synchronize();

        return std::move(n);
    }
}
