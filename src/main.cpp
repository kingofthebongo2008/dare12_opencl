#include "precompiled.h"

#include <cstdint>
#include <memory>

#include <os/windows/com_initializer.h>
#include <fs/fs_media.h>

#include "imaging/imaging_utils.h"
#include "imaging/imaging_opencl.h"

#include <opencl/opencl_platform.h>
#include <CL/cl.hpp>

#include <opencl/opencl_command_queue.h>
#include <opencl/opencl_buffer.h>

#include "opencl/opencl_grayscale.h"
#include "opencl/opencl_imaging_cpp.h"

#include "freeform_grayscale.h"

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

    auto d          = create_device(opencl::gpu, opencl::amd);
    auto ctx        = d->create_context(  );
    auto queue      = ctx->create_command_queue( );


    //read the png texture
    auto texture = imaging::read_texture(url0.get_path());
    auto pixels  = texture.get_pixels();
    auto pixels_cpu1 = pixels.get_pixels_cpu();

    auto color = freeform::create_color_texture(ctx.get(), texture);
    auto grayscale = freeform::create_grayscale_texture( ctx.get(), queue.get(), color );
    
    auto pixels1     = grayscale.get_pixels(queue.get());
    auto pixels_cpu2 = pixels1.get_pixels_cpu();

    
       

    return 0;

}






