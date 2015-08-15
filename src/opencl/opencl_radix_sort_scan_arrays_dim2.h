#pragma once

#include <cstdint>
#include <tuple>

#include <CL/cl.h>

#include <opencl/opencl_error.h>
#include <opencl/opencl_program.h>
#include <opencl/opencl_program_helpers.h>

namespace freeform
{
    namespace details
    {
        #include "opencl_radix_sort_scan_arrays_dim2_compiled.h"

        inline std::tuple<const uint8_t*, size_t> get_radix_sort_scan_arrays_dim2_ir(void)
        {
            return std::make_tuple(reinterpret_cast<const uint8_t*> (&opencl_radix_sort_scan_arrays_dim2[0]), sizeof(opencl_radix_sort_scan_arrays_dim2) );
        }


        inline cl_program create_radix_sort_scan_arrays_dim2_kernel( cl_context context, cl_device_id device )
        {
            cl_int binary_status = 0;
            cl_int errcode_ret = 0;

            auto binary = details::get_radix_sort_scan_arrays_dim2_ir();
            auto length = std::get<1>(binary);
            auto code = std::get<0 >(binary);

            return opencl::create_program( context, device, code, length);
        }
    }

    inline std::unique_ptr< opencl::program>  create_radix_sort_scan_arrays_dim2_kernel(const opencl::context * ctx )
    {
        auto program = details::create_radix_sort_scan_arrays_dim2_kernel(*ctx, ctx->get_device());
        return std::make_unique< opencl::program > (  program, false  );
    }
}


