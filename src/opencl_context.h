#pragma once

#include <utility>

#include <CL/cl.h>

#include <util/util_noncopyable.h>

#include "opencl_error.h"

namespace opencl
{
    class context : public util::noncopyable
    {
    public:

        context() : m_context(nullptr)
        {

        }

        context(cl_context context)
        {
            throw_if_failed(clRetainContext(context));
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

    private:

        cl_context m_context;
    };
}
