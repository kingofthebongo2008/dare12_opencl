#pragma once

#include <cstdint>
#include <utility>
#include <memory>

#include <CL/cl.h>

#include <util/util_noncopyable.h>

#include <opencl/opencl_error.h>

namespace opencl
{
    namespace details
    {
        inline void set_argument(cl_kernel kernel, uint32_t index, size_t size, const void* value)
        {
            throw_if_failed(clSetKernelArg(kernel, index, size, value));
        }
    }

    class kernel : public util::noncopyable
    {
    public:

        kernel() : m_kernel(nullptr)
        {

        }

        template < bool retain = true >
        explicit kernel( cl_kernel kernel )
        {
            if (retain)
            {
                throw_if_failed(clRetainKernel(kernel));
            }
            m_kernel = kernel;

        }

        explicit kernel(cl_kernel kernel, bool retain)
        {
            if (retain)
            {
                throw_if_failed(clRetainKernel(kernel));
            }
            m_kernel = kernel;

        }

        ~kernel()
        {
            if (m_kernel != nullptr)
            {
                clReleaseKernel(m_kernel);
            }
        }

        kernel(kernel&& o) : m_kernel(std::move(o.m_kernel))
        {
            o.m_kernel = nullptr;
        }

        kernel& operator=(kernel&& o)
        {
            if (this != &o)
            {
                if (m_kernel)
                {
                    clReleaseKernel(m_kernel);
                }

                m_kernel = std::move(o.m_kernel);
                o.m_kernel = nullptr;
            }

            return *this;
        }

        operator cl_kernel() const
        {
            return m_kernel;
        }

        void set_argument( uint32_t index, size_t size, const void* value )
        {
            details::set_argument(m_kernel, index, size, value);
        }

        template <typename t> void set_argument( uint32_t index, const t value )
        {
            set_argument(index, sizeof(t), &value) ;
        }

        void set_argument(uint32_t index, const uint32_t value)
        {
            set_argument(index, sizeof(value), &value);
        }
        
        void set_argument(uint32_t index, float value)
        {
            set_argument(index, sizeof(value), &value);
        }

        void set_argument(uint32_t index, cl_mem value)
        {
            set_argument(index, sizeof(value), &value);
        }

    private:

        cl_kernel m_kernel;
    };

}
