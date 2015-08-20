#pragma once

#include <CL/cl.hpp>

#include <bolt/cl/control.h>

#include <opencl/opencl_context.h>
#include <opencl/opencl_command_queue.h>


namespace freeform
{
    //holds common opencl structures
    class context
    {
        opencl::context*        m_context;
        opencl::command_queue*  m_command_queue;

        cl::CommandQueue        m_control_queue;
        bolt::cl::control       m_control;

        public:
            context(opencl::context*        context, opencl::command_queue*  queue) :
                m_context(context)
                , m_command_queue(queue)
                , m_control_queue((cl_command_queue) *queue, true )
                , m_control(m_control_queue)

            {

            }

            const bolt::cl::control& get_control() const
            {
                return m_control;
            }

            bolt::cl::control& get_control()
            {
                return m_control;
            }

            opencl::command_queue* get_queue() const
            {
                return m_command_queue;
            }

            opencl::context*       get_context() const
            {
                return m_context;
            }

            void launch1d(const opencl::kernel* kernel, uint32_t x) const
            {
                get_queue()->launch1d( kernel, x );
            }

            void launch1d(const opencl::kernel* kernel, uint32_t x, uint32_t local_x) const
            {
                get_queue()->launch1d(kernel, x, local_x);
            }

            void launch2d(const opencl::kernel* kernel, uint32_t x, uint32_t y) const
            {
                get_queue()->launch2d( kernel, x, y );
            }

            void copy(cl_mem src, cl_mem dst)
            {
                get_queue()->copy(src, dst);
            }

            void synchronize() const
            {
                get_queue()->synchronize();
            }
    };
}
