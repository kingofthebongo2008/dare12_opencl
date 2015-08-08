#pragma once

#include <exception>

#include <CL/cl.h>

namespace opencl
{
    class exception : public std::exception
    {
    public:
        exception(cl_int opencl_error) : std::exception("opencl exception")
        {
            m_opencl_error = opencl_error;
        }

    private:
        cl_int m_opencl_error;
        exception& operator=(const exception&);
    };


    template < typename exception > void throw_if_failed(cl_int opencl_error)
    {
        if (opencl_error != CL_SUCCESS)
        {
            throw exception(opencl_error);
        }
    }

    inline void throw_if_failed(cl_int opencl_error)
    {
        throw_if_failed<exception>(opencl_error);
    }
}
