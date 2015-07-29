#pragma once

#include <utility>

#include <CL/cl.h>

#include <util/util_noncopyable.h>

#include "opencl_error.h"

namespace opencl
{
    class device : public util::noncopyable
    {
        public:

        device() : m_device(nullptr)
        {

        }

        device( cl_device_id device )
        {
            throw_if_failed(clRetainDevice(m_device));
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

        private:

        cl_device_id m_device;
    };
}
