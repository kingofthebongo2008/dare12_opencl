#include "opencl_stdint.h"

#include "opencl_math_functions.h"
#include "opencl_freeform_patch.h"
#include "opencl_imaging.h"

/*
struct deform_points_kernel2
{
    const   cuda::image_kernel_info m_sampler;
    const   uint8_t*                m_grad;


    deform_points_kernel2(const cuda::image_kernel_info& sampler, const uint8_t* grad) : m_sampler(sampler), m_grad(grad)
    {

    }



    __device__ thrust::tuple<point, uint8_t> operator() (const thrust::tuple < point, point > p )
    {
        using namespace cuda;

        auto pt = thrust::get<0>(p);
        auto normal = thrust::get<1>(p);

        auto w = m_sampler.width();
        auto h = m_sampler.height();

        auto pixel_size = max(1.0f / w, 1.0f / h);
        auto pixel_size_y = 1.0f / h;
        auto scale = 1.5f;
        auto k1 = make_point(scale * pixel_size, scale * pixel_size);   //adjust this for faster convergence

        auto d0 = mad(k1, normal, pt);

        auto x = d0.x * w;
        auto y = d0.y * h;

        const uint8_t* pix00 = sample_2d< uint8_t, border_type::clamp >(m_grad, m_sampler, x - 1, y - 1);
        const uint8_t* pix01 = sample_2d< uint8_t, border_type::clamp >(m_grad, m_sampler, x - 0, y - 1);
        const uint8_t* pix02 = sample_2d< uint8_t, border_type::clamp >(m_grad, m_sampler, x + 1, y - 1);

        const uint8_t* pix10 = sample_2d< uint8_t, border_type::clamp>(m_grad, m_sampler, x - 1, y);
        const uint8_t* pix11 = sample_2d< uint8_t, border_type::clamp>(m_grad, m_sampler, x - 0, y);
        const uint8_t* pix12 = sample_2d< uint8_t, border_type::clamp>(m_grad, m_sampler, x + 1, y);

        const uint8_t* pix20 = sample_2d< uint8_t, border_type::clamp >(m_grad, m_sampler, x - 1, y + 1);
        const uint8_t* pix21 = sample_2d< uint8_t, border_type::clamp >(m_grad, m_sampler, x - 0, y + 1);
        const uint8_t* pix22 = sample_2d< uint8_t, border_type::clamp >(m_grad, m_sampler, x + 1, y + 1);

        auto  u00 = *pix00;
        auto  u01 = *pix01;
        auto  u02 = *pix02;

        auto  u10 = *pix10;
        auto  u11 = *pix11;
        auto  u12 = *pix12;

        auto  u20 = *pix20;
        auto  u21 = *pix21;
        auto  u22 = *pix22;

        auto mx = max(u00, u01);
        mx = max(mx, u02);
        mx = max(mx, u10);
        mx = max(mx, u11);
        mx = max(mx, u12);
        mx = max(mx, u20);
        mx = max(mx, u21);
        mx = max(mx, u22);

        //mx = *pix11;
        uint32_t stop = 0;

        if (mx > 250 || x > (w - 4) || y > (h - 4) || x < 4 || y < 4)
        {
            d0 = pt;
            stop = 1;
        }
        else
        {
            auto muls = 1.0f / 255.0f;

            auto u10f = u10 * muls;
            auto u12f = u12 * muls;

            auto u21f = u21 * muls;
            auto u01f = u01 * muls;

            auto gradx = u12f - u10f;
            auto grady = u21f - u01f;

            auto m = max(abs(gradx), abs(grady));
            auto n = 1.0f / (m + 0.001f);

            gradx = gradx * n;
            grady = grady * n;

            point up = make_point(gradx, grady);

            float scale_x = 2.6f;
            float scale_y = 2.6f;

            auto k1 = make_point(scale * pixel_size, scale * pixel_size);   //adjust this for faster convergence
            auto k2 = make_point(scale_x * pixel_size, scale_y * pixel_size);
            auto grad_pt = make_point(gradx, grady);

            d0 = mad(k1, normal, pt);
            d0 = mad(k2, grad_pt, d0);

              float s = 0.002f;
            d0 = add( mul( s,up ), d0 );
            
        }

        return thrust::make_tuple(d0, stop);
    }
};
*/

kernel void kernel_main( const read_only image_kernel_info sampler, const global read_only uint8_t* image, const global read_only freeform_point* points_in, const global read_only freeform_point* normals, global write_only freeform_point* points_out, global write_only uint32_t* stop)
{

}



