#include "opencl_stdint.h"
#include "opencl_imaging.h"

struct rgb
{
    uint8_t b;
    uint8_t g;
    uint8_t r;
};


kernel void kernel_main(const global read_only uint8_t* rgb_t, global write_only uint8_t* grayscale, const constant image_kernel_info* src)
{

/*
    using namespace cuda;
    auto x = blockIdx.x * blockDim.x + threadIdx.x;
    auto y = blockIdx.y * blockDim.y + threadIdx.y;

    if ( is_in_interior ( src, x, y ) )
    {
        const rgb* rgb_ = sample_2d<rgb>(rgb_t, src, x, y);
        auto  r = rgb_->r / 255.0f;
        auto  g = rgb_->g / 255.0f;
        auto  b = rgb_->b / 255.0f;

        auto  gray = 0.2989f * ( r  * r )  + 0.5870f * ( g *  g ) + 0.1140f * ( b  *  b ) ;
        auto  gray_quantized = static_cast<uint8_t> ( sqrtf( gray ) * 255.0f );
            
        write_2d(grayscale, dst, x, y, gray_quantized);
    }
*/

    size_t x = get_global_id(0);
    size_t y = get_global_id(1);

    const image_kernel_info* src2 = (const image_kernel_info*) src;

    if (is_in_interior(src2, x, y))
    {

    }


    grayscale[0] = 0;
}


