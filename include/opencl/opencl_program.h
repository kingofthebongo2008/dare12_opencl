#pragma once

#include <utility>
#include <memory>

#include <CL/cl.h>

#include <util/util_noncopyable.h>

#include <opencl/opencl_error.h>
#include <opencl/opencl_kernel.h>

namespace opencl
{
    namespace details
    {
    
    }


    class program : public util::noncopyable
    {
    public:

        program() : m_program(nullptr)
        {

        }

        template < bool retain = true >
        program( cl_program program )
        {
            if (retain)
            {
                throw_if_failed(clRetainProgram(program));
            }
            m_program = program;

        }

        program(cl_program program, bool retain)
        {
            if (retain)
            {
                throw_if_failed(clRetainProgram(program));
            }
            m_program = program;

        }

        ~program()
        {
            if (m_program != nullptr)
            {
                clReleaseProgram(m_program);
            }
        }

        program(program&& o) : m_program(std::move(o.m_program))
        {
            o.m_program = nullptr;
        }

        program& operator=(program&& o)
        {
            if (this != &o)
            {
                if (m_program)
                {
                    clReleaseProgram(m_program);
                }

                m_program = std::move(o.m_program);
                o.m_program = nullptr;
            }

            return *this;
        }

        operator cl_program() const
        {
            return m_program;
        }

        inline std::unique_ptr<kernel> create_kernel(const char* name)
        {
            cl_int      error_code = 0;
            cl_kernel   k = clCreateKernel(m_program, name, &error_code);

            throw_if_failed(error_code);
            auto         r = std::make_unique<kernel>(k, false);

            return std::move(r);
        }

    private:

        cl_program m_program;
    };

}
