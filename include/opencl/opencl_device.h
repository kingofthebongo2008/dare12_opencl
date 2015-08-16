#pragma once

#include <utility>

#include <CL/cl.h>

#include <util/util_noncopyable.h>

#include <opencl/opencl_error.h>
#include <opencl/opencl_context.h>

namespace opencl
{
    struct work_group_size
    {
        size_t m_x;
        size_t m_y;
        size_t m_z;
    };

    namespace details
    {
        inline std::unique_ptr<context> create_context(cl_device_id id)
        {
            cl_int       error_code = 0;

            cl_context   ctx = clCreateContext(nullptr, 1, &id, nullptr, nullptr, &error_code);

            throw_if_failed(error_code);

            auto         r = std::make_unique<context>(ctx, false);

            return std::move(r);
        }

        inline work_group_size get_max_work_group_size(cl_device_id device)
        {
            work_group_size s;
            size_t          real_size;
            throw_if_failed(clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(s), &s, &real_size));
            return s;
        }
    }

    class device : public util::noncopyable
    {
        public:

        device() : m_device(nullptr)
        {

        }

        template < bool retain = true > 
        device( cl_device_id device )
        {
            if (retain)
            {
                throw_if_failed(clRetainDevice(device));
            }
            m_device = device;

        }

        device(cl_device_id device, bool retain)
        {
            if (retain)
            {
                throw_if_failed(clRetainDevice(device));
            }
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

        work_group_size get_max_work_group_size() const
        {
            return details::get_max_work_group_size(m_device);
        }

        private:

        cl_device_id m_device;
    };
}
