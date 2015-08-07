#include "precompiled.h"

#include <cstdint>
#include <memory>

#include <os/windows/com_initializer.h>
#include <fs/fs_media.h>

#include "imaging_utils.h"

#include "opencl_ptr.h"
#include "opencl_platform.h"
#include <CL/cl.hpp>

#include "opencl_command_queue.h"
#include "opencl_buffer.h"

#include "opencl/opencl_grayscale.h"
#include "opencl/opencl_imaging_cpp.h"

int32_t main( int argc, char const* argv[] )
{
    using namespace     os::windows;
    com_initializer     com;

    fs::media_source source(L"../../../media/");

    using namespace opencl;

    auto url0 = fs::build_media_url(source, L"basic2_obstacles.png");
    auto url1 = fs::build_media_url(source, L"basic1_obstacles.png");
    auto url2 = fs::build_media_url(source, L"basic2_obstacles_grayscale.png");
    auto url3 = fs::build_media_url(source, L"basic2_obstacles_canny.png");

    auto d          = create_device(opencl::cpu, opencl::amd);
    auto ctx        = d->create_context(  );
    auto queue      = ctx->create_command_queue( );

    auto program    = freeform::create_grayscale_kernel( ctx.get() );
    auto kernel     = program->create_kernel("kernel_main");

    opencl::image_kernel_info info(5, 5, 5, 5, 5);

    kernel->set_argument(0, info);
    kernel->set_argument(1, info);

    queue->launch1d( kernel.get(), 20);
    queue->synchronize();

    //read the png texture
    auto texture = imaging::read_texture(url0.get_path());
    auto pixels  = texture.get_pixels();
    auto pixels_cpu1 = pixels.get_pixels_cpu();

    
    //create opencl buffers
    std::shared_ptr< opencl::buffer> buffer_shared( ctx->create_buffer< buffer::read_only | buffer::alloc_host_pointer| buffer::copy_host_pointer >(texture.get_size(), pixels.get_pixels_cpu()) );
    
    imaging::opencl_texture t(texture.get_width(), texture.get_height(), texture.get_bpp(), texture.get_size(), texture.get_pitch(), texture.get_image_type(), imaging::opencl_texture_storage( buffer_shared ) );

    auto pixels1     = t.get_pixels(queue.get());
    auto pixels_cpu2 = pixels1.get_pixels_cpu();

    auto r = std::memcmp(pixels_cpu1, pixels_cpu2, t.get_size());
       

    return 0;

}






