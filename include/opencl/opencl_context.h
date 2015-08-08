#pragma once

#include <utility>

#include <CL/cl.h>

#include <util/util_noncopyable.h>

#include <opencl/opencl_error.h>
#include <opencl/opencl_buffer.h>
#include <opencl/opencl_command_queue.h>

namespace opencl
{
    namespace details
    {
        cl_uint translate_flags(uint32_t flags)
        {
            cl_uint r = 0;

            r |= (flags & buffer::read_write) ? CL_MEM_READ_WRITE : 0;
            r |= (flags & buffer::write_only) ? CL_MEM_WRITE_ONLY : 0;
            r |= (flags & buffer::read_only) ? CL_MEM_READ_ONLY : 0;
            r |= (flags & buffer::use_host_pointer) ? CL_MEM_USE_HOST_PTR : 0;
            r |= (flags & buffer::alloc_host_pointer) ? CL_MEM_ALLOC_HOST_PTR : 0;
            r |= (flags & buffer::copy_host_pointer) ? CL_MEM_COPY_HOST_PTR : 0;

            return r;
        }

        template <uint32_t flags >
        cl_uint translate_flags()
        {
            cl_uint r = 0;

            r |= (flags & buffer::read_write) ? CL_MEM_READ_WRITE : 0;
            r |= (flags & buffer::write_only) ? CL_MEM_WRITE_ONLY : 0;
            r |= (flags & buffer::read_only) ? CL_MEM_READ_ONLY : 0;
            r |= (flags & buffer::use_host_pointer) ? CL_MEM_USE_HOST_PTR : 0;
            r |= (flags & buffer::alloc_host_pointer) ? CL_MEM_ALLOC_HOST_PTR : 0;
            r |= (flags & buffer::copy_host_pointer) ? CL_MEM_COPY_HOST_PTR : 0;

            return r;
        }

        inline std::unique_ptr<buffer> create_buffer(cl_context ctx, size_t size, uint32_t flags)
        {
            cl_int      error_code = 0;

            auto        r = clCreateBuffer(ctx, details::translate_flags(flags), size, nullptr, &error_code);

            throw_if_failed(error_code);

            auto    r1 = std::make_unique<buffer>(r, false);

            return std::move(r1);
        }

        inline std::unique_ptr<buffer> create_buffer(cl_context ctx, size_t size, uint32_t flags, void* host_pointer)
        {
            cl_int      error_code = 0;

            auto        r = clCreateBuffer(ctx, details::translate_flags(flags), size, host_pointer, &error_code);

            throw_if_failed(error_code);

            auto    r1 = std::make_unique<buffer>(r, false);

            return std::move(r1);
        }

        template <uint32_t flags>
        inline std::unique_ptr<buffer> create_buffer(cl_context ctx, size_t size)
        {
            cl_int      error_code = 0;

            auto        r = clCreateBuffer(ctx, details::translate_flags<flags>(), size, nullptr, &error_code);

            throw_if_failed(error_code);

            auto    r1 = std::make_unique<buffer>(r, false);

            return std::move(r1);
        }

        template <uint32_t flags>
        inline std::unique_ptr<buffer> create_buffer(cl_context ctx, size_t size, void* host_pointer)
        {
            cl_int      error_code = 0;

            auto        r = clCreateBuffer(ctx, details::translate_flags<flags>(), size, host_pointer, &error_code);

            throw_if_failed(error_code);

            auto    r1 = std::make_unique<buffer>(r);

            return std::move(r1);
        }

        inline std::unique_ptr<command_queue> create_command_queue(const cl_device_id device_id, cl_context   ctx)
        {
            cl_int       error_code = 0;

            cl_command_queue   queue = clCreateCommandQueue(ctx, device_id, 0, &error_code);

            throw_if_failed(error_code);

            auto         r = std::make_unique<command_queue>(queue, false);

            return std::move(r);
        }

        inline cl_device_id get_device(cl_context ctx)
        {
            cl_device_id id = nullptr;
            size_t       ret_size = 0;
            throw_if_failed(clGetContextInfo(ctx, CL_CONTEXT_DEVICES, sizeof(id), &id, &ret_size));
            return id;
        }
    }


    class context : public util::noncopyable
    {
    public:

        context() : m_context(nullptr)
        {

        }

        template < bool retain = true >
        context( cl_context context )
        {
            if (retain)
            {
                throw_if_failed(clRetainContext(context));
            }
            m_context = context;

        }

        context(cl_context context, bool retain)
        {
            if (retain)
            {
                throw_if_failed(clRetainContext(context));
            }
            m_context = context;

        }

        ~context()
        {
            if (m_context != nullptr)
            {
                clReleaseContext(m_context);
            }
        }

        context(context&& o) : m_context(std::move(o.m_context))
        {
            o.m_context = nullptr;
        }

        context& operator=(context&& o)
        {
            if (this != &o)
            {
                if (m_context)
                {
                    clReleaseContext(m_context);
                }

                m_context = std::move(o.m_context);
                o.m_context = nullptr;
            }

            return *this;
        }

        operator cl_context() const
        {
            return m_context;
        }

        template <uint32_t flags>
        inline std::unique_ptr<buffer> create_buffer(size_t size, void* host_pointer)
        {
            return details::create_buffer<flags>(m_context, size, host_pointer);
        }

        template <uint32_t flags>
        inline std::unique_ptr<buffer> create_buffer(size_t size)
        {
            return details::create_buffer<flags>(m_context, size);
        }

        inline std::unique_ptr<command_queue> create_command_queue()
        {
            return details::create_command_queue(details::get_device(m_context), m_context);
        }

        inline cl_device_id get_device() const
        {
            return details::get_device(m_context);
        }

    private:

        cl_context m_context;
    };
}
