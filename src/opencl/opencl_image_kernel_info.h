#pragma once

#include "opencl_stdint.h"

typedef struct _image_kernel_info
{
    size_t      m_bpp;
    size_t      m_size;

    uint32_t    m_pitch;
    uint32_t    m_width;
    uint32_t    m_height;
} image_kernel_info;

inline image_kernel_info image_kernel_info_create( size_t bpp, size_t size, uint32_t pitch, uint32_t width, uint32_t height )
{
    image_kernel_info r;
   
    r.m_bpp = (bpp);
    r.m_size = (size);
    r.m_pitch = (pitch);
    r.m_width = (width);
    r.m_height = (height);

    return r;
}

size_t image_kernel_info_bpp( const image_kernel_info* r)
{
    return r->m_bpp;
}

uint32_t image_kernel_info_pitch(const image_kernel_info* r)
{
    return r->m_pitch;
}


size_t  image_kernel_info_size(const image_kernel_info* r)
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




/*

enum border_type : int32_t
{
    clamp = 0
};

template <typename texture > inline image_kernel_info create_image_kernel_info(const texture& t)
{
    return image_kernel_info( t.get_bpp(), t.get_size(), t.get_pitch(), t.get_width(), t.get_height() );
}

template <imaging::image_type t> inline imaging::cuda_texture create_cuda_texture( uint32_t width, uint32_t height )
{
    auto bpp = imaging::get_bpp<t>();
    auto row_pitch = ( bpp * width + 7) / 8;
    auto size = row_pitch * height;

    //allocate memory buffer
    auto memory_buffer = cuda::make_memory_buffer( size );
    return imaging::cuda_texture(width, height, bpp, size, row_pitch, t, reinterpret_cast<uint8_t*> (memory_buffer->reset()));
}

__device__ inline bool is_in_interior(const image_kernel_info& info, uint32_t x, uint32_t y)
{
    return (x < info.width() && y < info.height() );
}

template < typename t > __device__ inline const t* sample_2d(const uint8_t * buffer, const image_kernel_info& info, uint32_t x, uint32_t y)
{
    return reinterpret_cast<const t*> (buffer + y * info.pitch() + x * sizeof(t));
}

template < typename t, border_type u > __device__ inline const t* sample_2d(const uint8_t * buffer, const image_kernel_info& info, uint32_t x, uint32_t y )
{
    //clamp to border

    x = min(info.width()  - 1 , x);
    x = max(0U, x);

    y = min( info.height() - 1 , y );
    y = max(0U, y);

    return reinterpret_cast<const t*> (buffer + y * info.pitch() + x * sizeof(t));
}

template < typename t > __device__ inline void write_2d( uint8_t * buffer, const image_kernel_info& info, uint32_t x, uint32_t y, t value )
{
    auto v = reinterpret_cast<t*> ( buffer + y * info.pitch() + x * sizeof(t) );
    *v = value;
}
*/

