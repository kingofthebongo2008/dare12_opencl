#include "opencl_stdint.h"
#include "opencl_imaging.h"

uint8_t    compute_sobel(
    uint8_t ul, // upper left
    uint8_t um, // upper middle
    uint8_t ur, // upper right
    uint8_t ml, // middle left
    uint8_t mm, // middle (unused)
    uint8_t mr, // middle right
    uint8_t ll, // lower left
    uint8_t lm, // lower middle
    uint8_t lr, // lower right
    float scale)
{
    int32_t horizontal  = ur + 2 * mr + lr - ul - 2 * ml - ll;
    int32_t vertical    = ul + 2 * um + ur - ll - 2 * lm - lr;

    int32_t sum = scale * ( abs(horizontal) + abs(vertical) ) ;

    sum = max( sum, 0);
    sum = min( sum, 0xff);

    return (sum);
}

kernel void kernel_main(const global read_only uint8_t* img_in, global write_only uint8_t* img_out, const read_only image_kernel_info src, const read_only image_kernel_info dst )
{
    uint32_t x = get_global_id(0);
    uint32_t y = get_global_id(1);

    if (is_in_interior(&src, x, y))
    {
        const global uint8_t* pix00 = sample_2d_clamp(img_in, &src, x - 1, y - 1, sizeof(uint8_t) );
        const global uint8_t* pix01 = sample_2d_clamp(img_in, &src, x - 0, y - 1, sizeof(uint8_t) );
        const global uint8_t* pix02 = sample_2d_clamp(img_in, &src, x + 1, y - 1, sizeof(uint8_t) );


        const global uint8_t* pix10 = sample_2d_clamp (img_in, &src, x - 1, y, sizeof(uint8_t) );
        const global uint8_t* pix11 = sample_2d_clamp (img_in, &src, x - 0, y, sizeof(uint8_t) );
        const global uint8_t* pix12 = sample_2d_clamp (img_in, &src, x + 1, y, sizeof(uint8_t) );

        const global uint8_t* pix20 = sample_2d_clamp(img_in, &src, x - 1, y + 1, sizeof(uint8_t) );
        const global uint8_t* pix21 = sample_2d_clamp(img_in, &src, x - 0, y + 1, sizeof(uint8_t) );
        const global uint8_t* pix22 = sample_2d_clamp(img_in, &src, x + 1, y + 1, sizeof(uint8_t) );

        uint8_t  u00 = *pix00;
        uint8_t  u01 = *pix01;
        uint8_t  u02 = *pix02;

        uint8_t  u10 = *pix10;
        uint8_t  u11 = *pix11;
        uint8_t  u12 = *pix12;

        uint8_t  u20 = *pix20;
        uint8_t  u21 = *pix21;
        uint8_t  u22 = *pix22;


        uint8_t  r = compute_sobel(
            u00, u01, u02,
            u10, u11, u12,
            u20, u21, u22, 1.0f
            );
            
        write_2d_uint8(img_out, &dst, x, y, r );
    }
}



