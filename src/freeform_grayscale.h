#pragma once

#include <opencl/opencl_context.h>
#include <opencl/opencl_command_queue.h>

#include "opencl/opencl_grayscale.h"
#include "opencl/opencl_imaging_cpp.h"

#include "imaging/imaging_utils.h"
#include "imaging/imaging_opencl.h"

namespace freeform
{
    inline imaging::opencl_texture create_color_texture ( opencl::context* ctx, const imaging::cpu_texture& texture )
    {
        using namespace opencl;
        std::shared_ptr< buffer > buffer_shared( ctx->create_buffer< buffer::read_only | buffer::alloc_host_pointer | buffer::copy_host_pointer >(texture.get_size(), texture.get_pixels().get_pixels_cpu()));
        imaging::opencl_texture t(texture.get_width(), texture.get_height(), texture.get_bpp(), texture.get_size(), texture.get_pitch(), texture.get_image_type(), imaging::opencl_texture_storage(buffer_shared));
        return std::move(t);
    }

    template <typename texture > inline opencl::image_kernel_info create_image_kernel_info(const texture& t)
    {
        return opencl::image_kernel_info(t.get_bpp(), t.get_size(), t.get_pitch(), t.get_width(), t.get_height());
    }


    inline imaging::opencl_texture create_grayscale_texture(opencl::context* ctx, opencl::command_queue* queue,  const imaging::opencl_texture& color)
    {
        using namespace opencl;
        using namespace imaging;

        auto program    = create_grayscale_kernel(ctx);
        auto kernel     = program->create_kernel("kernel_main");
        auto width      = color.get_width();
        auto height     = color.get_height();

        auto grayscale = create_opencl_texture<image_type::grayscale>(ctx, width, height );

        kernel->set_argument(0, (cl_mem) color.get_storage() );
        kernel->set_argument(1, (cl_mem)grayscale.get_storage());
        kernel->set_argument(2, create_image_kernel_info(color) );
        kernel->set_argument(3, create_image_kernel_info(grayscale) );

        
        queue->launch2d(kernel.get(), width, height);
        queue->synchronize();
        
        return std::move(grayscale);
    }
}
