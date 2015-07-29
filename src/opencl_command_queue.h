#pragma once

#include <utility>

#include <CL/cl.h>

#include <util/util_noncopyable.h>

#include "opencl_error.h"

namespace opencl
{
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

        private:

        cl_command_queue m_command_queue;
    };
}
