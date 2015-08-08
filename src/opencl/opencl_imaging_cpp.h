#pragma once

#include "opencl_image_kernel_info.h"

namespace opencl
{
    class image_kernel_info : public ::image_kernel_info
    {
        public:

        image_kernel_info(size_t bpp, size_t size, uint32_t pitch, uint32_t width, uint32_t height)
        {
            this->m_bpp     = bpp;
            this->m_size    = size;
            this->m_pitch   = pitch;
            this->m_width   = width;
            this->m_height = height;
        }

        size_t bpp() const
        {
            return image_kernel_info_bpp(this);
        }

        uint32_t pitch() const
        {
            return image_kernel_info_pitch(this);
        }

        size_t  size() const
        {
            return image_kernel_info_size(this);
        }

        uint32_t  width() const
        {
            return image_kernel_info_width(this);
        }

        uint32_t  height() const
        {
            return image_kernel_info_height(this);
        }

        private:
    };
}



