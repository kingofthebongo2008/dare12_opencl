#pragma once

#include <utility>

#include <CL/cl.h>

#include <util/util_noncopyable.h>

#include "opencl_error.h"
#include "opencl_buffer.h"

namespace opencl
{
    namespace details
    {
        inline void* map_buffer_write(cl_command_queue queue, cl_mem buffer, size_t size)
        {
            cl_int error_code = 0;

            void* r = clEnqueueMapBuffer(queue, buffer, true, CL_MAP_WRITE, 0, size, 0, nullptr, nullptr, &error_code);

            throw_if_failed(error_code);

            return r;
        }

        inline void unmap_buffer_write( cl_command_queue queue, cl_mem buffer, void* mapped_ptr )
        {
            cl_int error_code = 0;
            size_t size = 0;

            throw_if_failed(clEnqueueUnmapMemObject(queue, buffer, mapped_ptr, 0, nullptr, nullptr));
        }
    }

    class command_queue : public util::noncopyable
    {
        public:

        command_queue() : m_command_queue(nullptr)
        {

        }

        command_queue( cl_command_queue command_queue )
        {
            throw_if_failed(clRetainCommandQueue(command_queue));
            m_command_queue = command_queue;

        }

        ~command_queue()
        {
            if (m_command_queue != nullptr)
            {
                clReleaseCommandQueue(m_command_queue);
            }
        }

        command_queue( command_queue&& o) : m_command_queue( std::move( o.m_command_queue ) )
        {
            o.m_command_queue = nullptr;
        }

        command_queue& operator=(command_queue&& o)
        {
            if ( this != &o )
            {
                if ( m_command_queue )
                {
                    clReleaseCommandQueue( m_command_queue );
                }

                m_command_queue = std::move(o.m_command_queue);
                o.m_command_queue = nullptr;
            }

            return *this;
        }

        operator cl_command_queue() const
        {
            return m_command_queue;
        }

        void* begin_buffer_write( const buffer* buffer ) const
        {
            return details::map_buffer_write(m_command_queue, *buffer, buffer->get_size());
        }
 
        void end_buffer_write(const buffer* buffer, void* mapped_pointer) const
        {
            return details::unmap_buffer_write(m_command_queue, *buffer, mapped_pointer);
        }

        private:

        cl_command_queue m_command_queue;
    };

    class scoped_buffer_writer
    {
        public:

            scoped_buffer_writer(const command_queue* queue, const buffer* buffer) : m_queue(queue), m_buffer(buffer)
        {
            m_data = queue->begin_buffer_write(m_buffer);
        }

        ~scoped_buffer_writer()
        {
            m_queue->end_buffer_write(m_buffer, m_data );
        }


        void* get_data() const
        {
            return m_data;
        }

        template <typename t> t* get_data() const
        {
            return reinterpret_cast<t*> (m_data);
        }

        private:
        const   command_queue* m_queue;
        const   buffer*        m_buffer;
        void*                  m_data;
    };
}
