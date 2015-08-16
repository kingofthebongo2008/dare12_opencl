#pragma once

#include <tuple>

#include <opencl/opencl_context.h>
#include <opencl/opencl_command_queue.h>

#include "freeform_common.h"
#include "freeform_context.h"

#include "opencl/opencl_freeform_bitonic_sort_by_key.h"
#include "freeform_print_utils.h"


namespace freeform
{
    namespace details
    {
        freeform_patch invalid_patch()
        {
            freeform_patch r;

            r.x0 = std::numeric_limits<float>::max();
            r.x1 = std::numeric_limits<float>::max();
            r.x2 = std::numeric_limits<float>::max();
            r.x3 = std::numeric_limits<float>::max();

            r.y0 = std::numeric_limits<float>::max();
            r.y1 = std::numeric_limits<float>::max();
            r.y2 = std::numeric_limits<float>::max();
            r.y3 = std::numeric_limits<float>::max();

            return r;
        }

        uint32_t get_next_power_of_2(uint32_t value)
        {
            uint32_t bytes = sizeof(value);

            value--;
            for (int i = 0; i < bytes; i++)
                value |= value >> (1 << i);
            value++;

            return value;
        }

        uint32_t get_power_of_2(uint32_t value)
        {
            uint32_t result = 0 ;
            for (auto temp = value; temp > 1; temp >>= 1)
            {
                ++result;
            }
            return result;
        }

        uint32_t invalid_key()
        {
            return std::numeric_limits<uint32_t>::max();
        }

    }
    inline patches sort_by_key( freeform::context* ctx, patches& p, bolt::cl::device_vector<uint32_t>& keys )
    {
        using namespace opencl;

        auto program    = create_freeform_bitonic_sort_by_key_kernel( ctx->get_context() );
        auto kernel     = program->create_kernel("kernel_main");

        auto s = p.size();

        //todo: does a copy, change the vector class
        p.resize( details::get_next_power_of_2(s), details::invalid_patch() );
        keys.resize(details::get_next_power_of_2(s), details::invalid_key());

        size_t global_threads    = keys.size() / 2;
        size_t local_threads     = 256;

        auto   info = kernel->get_work_group_info();

        local_threads = std::min<size_t>(info.m_kernel_work_group_size, 256U);

        kernel->set_argument(0, keys.getBuffer());
        kernel->set_argument(1, p.getBuffer());
        kernel->set_argument(4, 1U);

        auto stages = details::get_power_of_2(details::get_next_power_of_2(s));

        for (auto stage = 0; stage < stages; ++stage)
        {
            kernel->set_argument(2, stage);

            for (auto pass = 0; pass < stage + 1; ++pass)
            {
                kernel->set_argument(3, pass);
                ctx->launch1d(kernel.get(), global_threads);
            }
        }

        
        ctx->synchronize();


        //todo: does a copy, change the vector class
        keys.resize(s);
        p.resize(s);

        return std::move(p);
    }
}
