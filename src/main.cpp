#include "precompiled.h"

#include <boost/intrusive_ptr.hpp>

#include <cstdint>

#include <os/windows/com_initializer.h>

#include <fs/fs_media.h>

#include "imaging_utils.h"

#include "opencl_ptr.h"

#include "opencl_platform.h"

int32_t main( int argc, char const* argv[] )
{
    using namespace     os::windows;
    com_initializer     com;

    fs::media_source source(L"../../../media/");

    auto url0 = fs::build_media_url(source, L"basic2_obstacles.png");
    auto url1 = fs::build_media_url(source, L"basic1_obstacles.png");
    auto url2 = fs::build_media_url(source, L"basic2_obstacles_grayscale.png");
    auto url3 = fs::build_media_url(source, L"basic2_obstacles_canny.png");

    auto d = opencl::create_opencl_device(opencl::gpu, opencl::intel);




    

    //read the png texture
    auto texture = imaging::read_texture(url0.get_path());
    auto pixels = texture.get_pixels();




    return 0;

}


