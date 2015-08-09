#include "opencl_stdint.h"
#include "opencl_imaging.h"

typedef struct _rgb
{
    uint8_t b;
    uint8_t g;
    uint8_t r;
} rgb;

kernel void kernel_main(const global read_only uint8_t* rgb_t, global write_only uint8_t* grayscale, const read_only image_kernel_info src, const read_only image_kernel_info dst)
{
    uint32_t x = get_global_id(0);
    uint32_t y = get_global_id(1);

    if (is_in_interior(&src, x, y))
    {
        const global rgb* rgb_ = (const global rgb*) sample_2d_clamp(rgb_t, &src, x, y, sizeof( rgb ) );
        float  r = rgb_->r / 255.0f;
        float  g = rgb_->g / 255.0f;
        float  b = rgb_->b / 255.0f;

        float  gray = 0.2989f * (r  * r) + 0.5870f * (g *  g) + 0.1140f * (b  *  b);
        uint8_t  gray_quantized = (uint8_t ) (sqrt(gray) * 255.0f);

        write_2d_uint8( grayscale, &dst, x, y, gray_quantized);
        //write_2d_uint8( grayscale, &dst, x, y, 0x1);
    }
    else
    {
        //write_2d_uint8( grayscale, &dst, x, y, image_kernel_info_width(&dst));
    }
}


