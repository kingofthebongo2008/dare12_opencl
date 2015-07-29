#pragma once

#include <memory>

#include "opencl_device.h"

#include <iostream>

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


    inline device* create_opencl_device(  device_type type, device_vendor vendor )
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

        }


        return nullptr;
    }
}
