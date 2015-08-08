#include "opencl_stdint.h"
#include "opencl_imaging.h"

typedef struct _rgb
{
    uint8_t b;
    uint8_t g;
    uint8_t r;
} rgb;


//kernel void kernel_main(const global read_only uint8_t* rgb_t, global write_only uint8_t* grayscale, const image_kernel_info src, const image_kernel_info dst)
kernel void kernel_main( global write_only uint8_t* grayscale,  image_kernel_info src, image_kernel_info dst )
{
    uint32_t x = get_global_id(0);
    uint32_t y = get_global_id(1);

         
    
    const image_kernel_info* src2 = (const image_kernel_info*) &src;
    const image_kernel_info* dst2 = (const image_kernel_info*) &dst;

    if (is_in_interior(src2, x, y))
    {
        write_2d_uint8( grayscale, dst2, x, y, 3);
    }
    
    
    write_2d_uint8( grayscale, dst2, 0, 0, 3);

    grayscale[0] = 3;

    /* 
    


    const uint8_t* rgb_t2 = (const uint8_t*)( rgb_t );
    if (is_in_interior(src2, x, y))
    {
        const rgb* rgb_ = sample_2d_clamp(rgb_t2, src2, x, y, sizeof( rgb ) );
        float  r = rgb_->r / 255.0f;
        float  g = rgb_->g / 255.0f;
        float  b = rgb_->b / 255.0f;

        float  gray = 0.2989f * (r  * r) + 0.5870f * (g *  g) + 0.1140f * (b  *  b);
        uint8_t  gray_quantized = (uint8_t ) (sqrt(gray) * 255.0f);

        write_2d_uint8( grayscale2, dst2, x, y, gray_quantized);
    }
    */
}


