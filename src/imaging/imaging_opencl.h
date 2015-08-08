#pragma once

#include <opencl/opencl_context.h>

namespace imaging
{
    template <imaging::image_type t> inline imaging::opencl_texture create_opencl_texture( opencl::context* ctx, uint32_t width, uint32_t height )
    {
        using namespace opencl;
        auto bpp = imaging::get_bpp<t>();
        auto row_pitch = (bpp * width + 7) / 8;
        auto size = row_pitch * height;

        //allocate memory buffer
        std::shared_ptr< buffer > buffer_shared(ctx->create_buffer< buffer::read_write | buffer::alloc_host_pointer >(size));

        return std::move(imaging::opencl_texture(width, height, bpp, size, row_pitch, t, imaging::opencl_texture_storage(buffer_shared)));
    }
}
