#pragma once

#include "opencl_stdint.h"

typedef struct _image_kernel_info
{
    uint32_t    m_bpp;
    uint32_t    m_size;

    uint32_t    m_pitch;
    uint32_t    m_width;
    uint32_t    m_height;

    uint32_t    m_padding[3];
} image_kernel_info;

inline image_kernel_info image_kernel_info_create(uint32_t bpp, uint32_t size, uint32_t pitch, uint32_t width, uint32_t height )
{
    image_kernel_info r;
   
    r.m_bpp = (bpp);
    r.m_size = (size);
    r.m_pitch = (pitch);
    r.m_width = (width);
    r.m_height = (height);

    return r;
}

uint32_t image_kernel_info_bpp( const image_kernel_info* r)
{
    return r->m_bpp;
}

uint32_t image_kernel_info_pitch(const image_kernel_info* r)
{
    return r->m_pitch;
}


uint32_t  image_kernel_info_size(const image_kernel_info* r)
{
    return r->m_size;
}

uint32_t  image_kernel_info_width(const image_kernel_info* r)
{
    return r->m_width;
}

uint32_t  image_kernel_info_height(const image_kernel_info* r)
{
    return r->m_height;
}




