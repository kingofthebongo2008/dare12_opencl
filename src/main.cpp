#include "precompiled.h"

#include <bolt/cl/device_vector.h>

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
#include "opencl/opencl_canny.h"
#include "opencl/opencl_imaging_cpp.h"

#include "freeform_grayscale.h"
#include "freeform_canny.h"

#include "freeform_init_freeform.h"
#include "freeform_split.h"
#include "freeform_context.h"

#include "opencl_sort_by_key.h"

namespace freeform
{
    void display(const imaging::opencl_texture& t, const opencl::command_queue* queue, const samples& p);
    void display(const imaging::opencl_texture& t, const opencl::command_queue* queue, const patches& p);
}


int32_t main( int argc, char const* argv[] )
{
    using namespace     os::windows;
    com_initializer     com;

    fs::media_source source(L"../../../media/");

    using namespace opencl;

    auto url0 = fs::build_media_url(source, L"essaisynth2.png");
    auto url1 = fs::build_media_url(source, L"essaisynth1.png");
    auto url2 = fs::build_media_url(source, L"essaisynth2_grayscale.png");
    auto url3 = fs::build_media_url(source, L"essaisynth2_canny.png");

    auto url   = fs::build_media_url(source, L"temp.png");
    auto url_1 = fs::build_media_url(source, L"temp1.png");

    auto d          = create_device(opencl::cpu, opencl::intel);
    auto ctx        = d->create_context();
    auto queue      = ctx->create_command_queue();

    //read the png texture
    auto texture     = imaging::read_texture(url0.get_path());
    auto pixels      = texture.get_pixels();
    auto pixels_cpu1 = pixels.get_pixels_cpu();

    auto color      = freeform::create_color_texture(ctx.get(), texture);
    auto grayscale  = freeform::create_grayscale_texture( ctx.get(), queue.get(), color );
    auto canny      = freeform::create_canny_texture(ctx.get(), queue.get(), grayscale, 0.5f);
    
    auto pixels1     = grayscale.get_pixels(queue.get());
    auto pixels_cpu2 = pixels1.get_pixels_cpu();

    imaging::write_texture( grayscale, url.get_path(), queue.get() );
    imaging::write_texture( canny, url_1.get_path(), queue.get());

    freeform::context  ff_ctx(ctx.get(), queue.get());

    auto center_image_x = 0.5f;// 341.0f / gray.get_width();
    auto center_image_y = 0.5f;// 240.0f / gray.get_height();

    auto pixel_size = std::max(1.0f / grayscale.get_width(), 1.0f / grayscale.get_height());
    auto radius = 20.0f * pixel_size;
    auto patch_count = 20;
    
    auto init  = freeform::initialize_freeform(&ff_ctx, center_image_x, center_image_y, radius, patch_count);
    auto split = freeform::split(&ff_ctx, std::get<1>(init), pixel_size );

    freeform::display(grayscale, queue.get(), std::get<1>(init));


    
    return 0;
}






