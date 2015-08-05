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
    uint32_t pitch = image_kernel_info_pitch(info);

    x = min(width - 1, x);
    x = max(0U, x);

    y = min(height - 1, y);
    y = max(0U, y);

    return  ( buffer + y * pitch + x * sizeof_t);
}

inline void write_2d_uint8( uint8_t * buffer, const image_kernel_info* info, uint32_t x, uint32_t y, uint8_t value )
{
    uint32_t pitch = image_kernel_info_pitch(info);

    uint8_t* v = (uint8_t*) (buffer + y * pitch + x * sizeof(value) );
    *v = value;
}



