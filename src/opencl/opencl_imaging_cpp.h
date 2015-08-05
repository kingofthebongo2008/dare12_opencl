#pragma once

#include "opencl_image_kernel_info.h"

namespace opencl
{
    class image_kernel_info
    {
        public:

        image_kernel_info(size_t bpp, size_t size, uint32_t pitch, uint32_t width, uint32_t height)
        {
            m_info = image_kernel_info_create(bpp, size, pitch, width, height);
        }

        size_t bpp(const image_kernel_info* r) const
        {
            return image_kernel_info_bpp(&m_info);
        }

        uint32_t pitch(const image_kernel_info* r) const
        {
            return image_kernel_info_pitch(&m_info);
        }

        size_t  size(const image_kernel_info* r) const
        {
            return image_kernel_info_size(&m_info);
        }

        uint32_t  width(const image_kernel_info* r) const
        {
            return image_kernel_info_width(&m_info);
        }

        uint32_t  height(const image_kernel_info* r) const
        {
            return image_kernel_info_height(&m_info);
        }

        private:

        ::image_kernel_info m_info;
    };
}

/*
template <typename texture > inline image_kernel_info create_image_kernel_info(const texture& t)
{
    return image_kernel_info(t.get_bpp(), t.get_size(), t.get_pitch(), t.get_width(), t.get_height());
}

template <imaging::image_type t> inline imaging::cuda_texture create_cuda_texture(uint32_t width, uint32_t height)
{
    auto bpp = imaging::get_bpp<t>();
    auto row_pitch = (bpp * width + 7) / 8;
    auto size = row_pitch * height;

    //allocate memory buffer
    auto memory_buffer = cuda::make_memory_buffer(size);
    return imaging::cuda_texture(width, height, bpp, size, row_pitch, t, reinterpret_cast<uint8_t*> (memory_buffer->reset()));
}


*/


