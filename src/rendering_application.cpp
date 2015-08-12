#include "precompiled.h"

#include <opencl/opencl_command_queue.h>

#include "patch_rendering_application.h"
#include "sample_rendering_application.h"


#include "imaging/imaging_utils.h"

#include "freeform_common.h"
#include "graphic_types.h"



namespace freeform
{
    template <typename t> graphic::patch_draw_info create_draw_info(const bolt::cl::device_vector<t> & p)
    {
        graphic::patch_draw_info di;
        di.m_patches.resize( p.size() );
        auto i = 0;

        for (uint32_t i = 0; i < p.size(); ++i )
        {
            graphic::patch pt;

            t p0 = p[i];

            pt.m_points[0].m_x = p0.x0;
            pt.m_points[1].m_x = p0.x1;
            pt.m_points[2].m_x = p0.x2;
            pt.m_points[3].m_x = p0.x3;

            pt.m_points[0].m_y = p0.y0;
            pt.m_points[1].m_y = p0.y1;
            pt.m_points[2].m_y = p0.y2;
            pt.m_points[3].m_y = p0.y3;

            di.m_patches[i++] = pt;
        }

        return std::move(di);
    }

    void display(const imaging::opencl_texture& t, const opencl::command_queue* queue, const patches& p, const graphic::transform_info& transform)
    {
        std::unique_ptr< rendering_application >  app(new patch_application(L"Patches", t, queue, create_draw_info<freeform_patch>(p), transform));
        app->run();
    }

    void display(const imaging::opencl_texture& t, const opencl::command_queue* queue, const samples& p, const graphic::transform_info& transform)
    {
        std::unique_ptr< rendering_application >  app(new sample_application(L"Samples", t, queue, create_draw_info<freeform_sample>(p), transform));
        app->run();
    }


    void    display(const imaging::opencl_texture& t, const opencl::command_queue* queue, const patches& p)
    {
        //make a transform to rescale to perspective space
        freeform::graphic::transform_info transform;
        transform.m_center_x = static_cast<float>(t.get_width());
        transform.m_center_y = static_cast<float>(t.get_height());
        transform.m_image_height = static_cast<float>(t.get_height());
        transform.m_image_width = static_cast<float>(t.get_width());

        display(t, queue, p, transform);
    }

    void    display(const imaging::opencl_texture& t, const opencl::command_queue* queue, const samples& p)
    {
        //make a transform to rescale to perspective space
        freeform::graphic::transform_info transform;
        transform.m_center_x = static_cast<float>(t.get_width());
        transform.m_center_y = static_cast<float>(t.get_height());
        transform.m_image_height = static_cast<float>(t.get_height());
        transform.m_image_width = static_cast<float>(t.get_width());

        display(t, queue, p, transform);
    }
}

