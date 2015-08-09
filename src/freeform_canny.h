#pragma once

#include <opencl/opencl_context.h>
#include <opencl/opencl_command_queue.h>

#include "opencl/opencl_imaging_cpp.h"

#include "imaging/imaging_utils.h"
#include "imaging/imaging_opencl.h"

namespace freeform
{
    inline imaging::opencl_texture create_canny_texture(opencl::context* ctx, opencl::command_queue* queue,  const imaging::opencl_texture& grayscale, float threshold )
    {
        /*
        using namespace opencl;
        using namespace imaging;

        auto program    = create_grayscale_kernel(ctx);
        auto kernel     = program->create_kernel("kernel_main");
        auto width      = color.get_width();
        auto height     = color.get_height();

        auto grayscale = create_opencl_texture<image_type::grayscale>(ctx, width, height );

        kernel->set_argument(0, (cl_mem) color.get_storage() );
        kernel->set_argument(1, (cl_mem) grayscale.get_storage() );
        kernel->set_argument(2, create_image_kernel_info(color) );
        kernel->set_argument(3, create_image_kernel_info(grayscale) );

        queue->launch2d(kernel.get(), width, height);
        queue->synchronize();
        */
        return std::move(grayscale);
    }
}
