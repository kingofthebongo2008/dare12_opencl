#pragma once

#include <memory>
#include <string>
#include <utility>

#include <CL/cl.h>

#include <util/util_noncopyable.h>

#include <opencl/opencl_error.h>
#include <opencl/opencl_kernel.h>

namespace opencl
{
    inline inline cl_program create_program(cl_context context, cl_device_id device, const uint8_t* code, size_t code_size)
    {
        cl_int binary_status = 0;
        cl_int errcode_ret = 0;

        //we expect spir here
        auto program = clCreateProgramWithBinary(context, 1, &device, &code_size, &code, &binary_status, &errcode_ret);

        opencl::throw_if_failed(errcode_ret);

        try
        {
            opencl::throw_if_failed(clBuildProgram(program, 1, &device, nullptr, nullptr, nullptr));
        }

        catch (const opencl::exception& e)
        {
            clBuildProgram(program, 1, &device, nullptr, nullptr, nullptr);
            char errors[1024];
            size_t size = 0;
            clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 1024, &errors[0], &size);
            throw opencl::build_program_exception(e.get_error_code(), std::string(&errors[0]));
        }

        return program;
    }
}
