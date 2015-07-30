#pragma once

#include <utility>

#include <CL/cl.h>

#include <util/util_noncopyable.h>

#include "opencl_error.h"
#include "opencl_context.h"

namespace opencl
{
    namespace details
    {
        inline std::unique_ptr<context> create_context(cl_device_id id)
        {
            cl_int       error_code = 0;

            cl_context   ctx = clCreateContext(nullptr, 1, &id, nullptr, nullptr, &error_code);

            throw_if_failed(error_code);

            auto         r = std::make_unique<context>(ctx);

            clReleaseContext(ctx);

            return std::move(r);
        }
    }

    class device : public util::noncopyable
    {
        public:

        device() : m_device(nullptr)
        {

        }

        device( cl_device_id device )
        {
            throw_if_failed(clRetainDevice(device));
            m_device = device;

        }

        ~device()
        {
            if (m_device != nullptr)
            {
                clReleaseDevice(m_device);
            }
        }

        device( device&& o) : m_device( std::move( o.m_device ) )
        {
            o.m_device = nullptr;
        }

        device& operator=(device&& o)
        {
            if ( this != &o )
            {
                if ( m_device )
                {
                    clReleaseDevice( m_device );
                }

                m_device = std::move(o.m_device);
                o.m_device = nullptr;
            }

            return *this;
        }

        operator cl_device_id() const
        {
            return m_device;
        }

        inline std::unique_ptr<context> create_context()
        {
            return details::create_context(m_device);
        }

        private:

        cl_device_id m_device;
    };
}
