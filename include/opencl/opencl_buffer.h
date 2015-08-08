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
        inline size_t get_size( cl_mem buffer )
        {
            size_t       size = 0;
            size_t       ret_size = 0;
            throw_if_failed(clGetMemObjectInfo(buffer, CL_MEM_SIZE, sizeof(size), &size, &ret_size));
            return size;
        }
    }

    class buffer : public util::noncopyable
    {
    public:

        enum flags : uint32_t
        {
            read_write          = 1 << 0,
            write_only          = 1 << 1,
            read_only           = 1 << 2,
            use_host_pointer    = 1 << 3,
            alloc_host_pointer  = 1 << 4,
            copy_host_pointer   = 1 << 5
        };


        buffer() : m_buffer(nullptr)
        {

        }

        template < bool retain = true >
        explicit buffer( cl_mem buffer )
        {
            if (retain)
            {
                throw_if_failed(clRetainMemObject(buffer));
            }
            m_buffer = buffer;

        }

        explicit buffer(cl_mem buffer, bool retain)
        {
            if (retain)
            {
                throw_if_failed(clRetainMemObject(buffer));
            }
            m_buffer = buffer;

        }

        ~buffer()
        {
            if (m_buffer != nullptr)
            {
                clReleaseMemObject(m_buffer);
            }
        }

        buffer(buffer&& o) : m_buffer(std::move(o.m_buffer))
        {
            o.m_buffer = nullptr;
        }

        buffer& operator=(buffer&& o)
        {
            if (this != &o)
            {
                if (m_buffer)
                {
                    clReleaseMemObject(m_buffer);
                }

                m_buffer = std::move(o.m_buffer);
                o.m_buffer = nullptr;
            }

            return *this;
        }

        operator cl_mem() const
        {
            return m_buffer;
        }

        size_t get_size() const
        {
            return details::get_size(m_buffer);
        }

    private:

        cl_mem m_buffer;
    };

}
