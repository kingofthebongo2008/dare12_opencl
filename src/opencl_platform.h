#pragma once

#include <memory>

#include "opencl_device.h"
#include "opencl_context.h"



namespace opencl
{
    enum device_type
    {
        gpu,
        cpu
    };

    enum device_vendor
    {
        amd,
        intel,
        nvidia
    };

    namespace details
    {
        bool matches(device_vendor vendor_required, const char* vendor_supplied)
        {
            if (vendor_required == intel)
            {
                const char* v = "Intel(R) Corporation";
                auto a = strlen(v);
                return strncmp(vendor_supplied, v, a) == 0;
            }

            if (vendor_required == nvidia)
            {
                const char* v = "NVIDIA Corporation";
                auto a = strlen(v);
                return strncmp(vendor_supplied, v, a) == 0;
            }

            if (vendor_required == amd)
            {
                const char* v = "Advanced Micro Devices, Inc.";
                auto a = strlen(v);
                return strncmp(vendor_supplied, v, a) == 0;
            }

            return false;
        }


    }


    inline std::unique_ptr<device> create_device(  device_type type, device_vendor vendor )
    {
        cl_uint num_platforms;

        throw_if_failed(clGetPlatformIDs(0, nullptr, &num_platforms) );

        std::unique_ptr< cl_platform_id[] > platforms(new cl_platform_id[num_platforms]);

        throw_if_failed(clGetPlatformIDs(num_platforms, &platforms[0], &num_platforms));
        
        for (auto i = 0; i < num_platforms; ++i)
        {
            size_t vendor_name_length;

            throw_if_failed(clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, 0, nullptr, &vendor_name_length));

            std::unique_ptr< char[] > vendor_name( new char[ vendor_name_length ] );

            throw_if_failed(clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, vendor_name_length, &vendor_name[0], &vendor_name_length));


            if (details::matches(vendor, &vendor_name[0] ))
            {
                cl_uint num_devices;

                cl_uint device_type = type == device_type::gpu ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU;

                throw_if_failed(clGetDeviceIDs(platforms[i], device_type, 0, nullptr, &num_devices));

                std::unique_ptr< cl_device_id[] > devices( new cl_device_id[num_devices] );

                throw_if_failed(clGetDeviceIDs(platforms[i], device_type, num_devices, &devices[0], &num_devices));
                
                return std::move(std::make_unique<device>(devices[0]));
            }
        }

        return nullptr;
    }


    inline std::unique_ptr<context> create_context(const device* device)
    {
        cl_device_id id = *device;
        cl_int       error_code = 0;

        cl_context   ctx = clCreateContext(nullptr, 1, &id, nullptr, nullptr, &error_code);

        throw_if_failed(error_code);
        
        auto         r = std::make_unique<context>(ctx);

        clReleaseContext(ctx);

        return std::move(r);

    }


    
}
