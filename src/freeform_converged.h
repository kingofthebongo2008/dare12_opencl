#pragma once

#include <tuple>

#include <bolt/cl/sort_by_key.h>

#include <opencl/opencl_context.h>
#include <opencl/opencl_command_queue.h>

#include "freeform_common.h"
#include "freeform_context.h"

#include "opencl/opencl_reduction.h"

#include "freeform_print_utils.h"



namespace freeform
{
    inline bool converged( freeform::context* ctx, bolt::cl::device_vector<uint32_t>& v)
    {
        auto program    = create_reduction_kernel( ctx->get_context() );
        auto kernel     = program->create_kernel("kernel_main");

        auto g = kernel->get_work_group_info();
        const uint32_t group_size   = 8; //g.m_kernel_work_group_size;, intel cpu does not seem to work correctly
        const uint32_t multiply     = 2; //Require because of extra addition before loading to local memory

        // make sure length is multiple of group size * 4
        uint32_t mulFactor = group_size *  multiply;
        uint32_t length = v.size();

        length = ( length < mulFactor) ? mulFactor : length;
        length = ( (length + mulFactor - 1 ) / mulFactor ) * mulFactor;

        v.resize(length);

        auto num_blocks = length / ( group_size * multiply );

        bolt::cl::device_vector<uint32_t> o(ctx->get_control());

        o.resize(num_blocks);
        
        kernel->set_argument(0, v.getBuffer() );
        kernel->set_argument(1, o.getBuffer() );
        kernel->set_argument(2, group_size, nullptr );
        
        ctx->launch1d(kernel.get(), length / multiply, group_size );
        ctx->synchronize();

        //todo: move this summation on the device with a kernel
        auto sum = 0U;
        for (auto i = 0; i < num_blocks; ++i)
        {
            sum += o[i];
        }

        return true;
    }
}
