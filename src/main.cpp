#include "precompiled.h"


#include <cstdint>
#include <memory>

#include <os/windows/com_initializer.h>

#include <fs/fs_media.h>

#include "imaging_utils.h"

#include "opencl_ptr.h"

#include "opencl_platform.h"

#include <CL/cl.hpp>

#include "opencl_context.h"


int32_t main( int argc, char const* argv[] )
{
    using namespace     os::windows;
    com_initializer     com;

    fs::media_source source(L"../../../media/");

    auto url0 = fs::build_media_url(source, L"basic2_obstacles.png");
    auto url1 = fs::build_media_url(source, L"basic1_obstacles.png");
    auto url2 = fs::build_media_url(source, L"basic2_obstacles_grayscale.png");
    auto url3 = fs::build_media_url(source, L"basic2_obstacles_canny.png");

    auto d = opencl::create_device(opencl::gpu, opencl::nvidia);
    auto ctx = opencl::create_context( d.get() );

    cl::Device d1( *d );




    

    //read the png texture
    //auto texture = imaging::read_texture(url0.get_path());
    //auto pixels = texture.get_pixels();




    return 0;

}






