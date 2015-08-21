#pragma once

#include <cstdint>
#include <tuple>

#include <CL/cl.h>

#include <opencl/opencl_error.h>
#include <opencl/opencl_program.h>

namespace freeform
{
    namespace details
    {
        #include "opencl_canny_compiled.h"

        inline std::tuple<const uint8_t*, size_t> get_canny_ir(void)
        {
            return std::make_tuple(reinterpret_cast<const uint8_t*> (&opencl_canny[0]), sizeof(opencl_canny) );
        }

        inline cl_program create_canny_kernel(cl_context context, cl_device_id device)
        {
            static cl_program p = nullptr;

            if (p == nullptr)
            {
                cl_int binary_status = 0;
                cl_int errcode_ret = 0;

                auto binary = details::get_canny_ir();
                auto length = std::get<1>(binary);
                auto code = std::get<0 >(binary);

                p = opencl::create_program(context, device, code, length);
            }

            opencl::throw_if_failed(clRetainProgram(p));
            return p;
        }
    }

    inline std::unique_ptr< opencl::program>  create_canny_kernel(const opencl::context * ctx )
    {
        auto program = details::create_canny_kernel(*ctx, ctx->get_device());
        return std::make_unique< opencl::program > (  program, false  );
    }
}


