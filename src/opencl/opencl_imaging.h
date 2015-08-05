#pragma once

#include "opencl_image_kernel_info.h"


static inline bool is_in_interior( const image_kernel_info* info, uint32_t x, uint32_t y)
{
    return ( x < image_kernel_info_width(info) && y < image_kernel_info_height(info));
}

inline const void* sample_2d(const uint8_t * buffer, const image_kernel_info* info, uint32_t x, uint32_t y, size_t sizeof_t)
{
    return  ( buffer + y * image_kernel_info_pitch( info ) + x * sizeof_t );
}

inline const void* sample_2d_clamp(const uint8_t * buffer, const image_kernel_info* info, uint32_t x, uint32_t y, size_t sizeof_t)
{
    uint32_t width  = image_kernel_info_width(info);
    uint32_t height = image_kernel_info_height(info);

    x = min(width - 1, x);
    x = max(0U, x);

    y = min(height - 1, y);
    y = max(0U, y);

    return  ( buffer + y * image_kernel_info_pitch(info) + x * sizeof_t);
}


/*

enum border_type : int32_t
{
    clamp = 0
};





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

