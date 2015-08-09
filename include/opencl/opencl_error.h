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

    cl_int get_error_code() const
    {
        return m_opencl_error;
    }

    private:
        cl_int m_opencl_error;
        exception& operator=(const exception&);
    };

    class build_program_exception : public exception
    {
        public:

        build_program_exception(cl_int opencl_error, const std::string build_error) : exception(opencl_error) , m_error(build_error)
        {

        }

        const char* what() const
        {
            return m_error.c_str();
        }

        private:

        cl_int m_opencl_error;
        std::string  m_error;

        build_program_exception& operator=(const build_program_exception&);
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
