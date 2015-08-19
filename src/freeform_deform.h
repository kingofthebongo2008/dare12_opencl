#pragma once

#include <tuple>

#include <opencl/opencl_context.h>
#include <opencl/opencl_command_queue.h>

#include "freeform_common.h"
#include "freeform_context.h"

#include "freeform_print_utils.h"

#include "opencl/opencl_freeform_deform_normal_curve_points.h"
#include "opencl/opencl_freeform_deform_scatter_normals.h"
#include "opencl/opencl_freeform_deform_scatter_points.h"
#include "opencl/opencl_freeform_deform_average_samples.h"
#include "opencl/opencl_freeform_deform_deform_points.h"
#include "opencl/opencl_freeform_deform_gather_samples.h"



namespace freeform
{
    namespace details
    {

    }

    inline samples deform_normal_curve_points(freeform::context* ctx, const patches& p)
    {
        samples                            s( ctx->get_control() ) ;

        s.resize(p.size());

        auto program = create_freeform_deform_normal_curve_points_kernel(ctx->get_context());
        auto kernel  = program->create_kernel("kernel_main");


        kernel->set_argument(0, p.getBuffer());
        kernel->set_argument(1, s.getBuffer());

        ctx->launch1d(kernel.get(), p.size());
        ctx->synchronize();
        return std::move(s);
    }

    inline points deform_scatter_normals(freeform::context* ctx, const samples& s)
    {
        points normal_vectors( ctx->get_control() );

        normal_vectors.resize(s.size() * 4) ;


        auto program = create_freeform_deform_scatter_normals_kernel(ctx->get_context());
        auto kernel = program->create_kernel("kernel_main");


        kernel->set_argument(0, s.getBuffer());
        kernel->set_argument(1, normal_vectors.getBuffer());

        ctx->launch1d(kernel.get(), s.size());
        ctx->synchronize();
        return std::move(normal_vectors);
    }

    inline points deform_scatter_points(freeform::context* ctx, const patches& s)
    {
        points pts(ctx->get_control());

        pts.resize(s.size() * 4);


        auto program = create_freeform_deform_scatter_points_kernel(ctx->get_context());
        auto kernel = program->create_kernel("kernel_main");


        kernel->set_argument(0, s.getBuffer());
        kernel->set_argument(1, pts.getBuffer());

        ctx->launch1d(kernel.get(), s.size());
        ctx->synchronize();
        return std::move(pts);
    }

    inline std::tuple<points,stops> deform_deform_points(freeform::context* ctx, const points& p, const points& normals, const imaging::opencl_texture& edges)
    {
        points resampled_points( ctx->get_control() );
        stops  stops(ctx->get_control());

        resampled_points.resize(p.size());
        stops.resize(p.size());

        auto program    = create_freeform_deform_deform_points_kernel( ctx->get_context() );
        auto kernel     = program->create_kernel( "kernel_main" );

        kernel->set_argument(0, create_image_kernel_info(edges));
        kernel->set_argument(1, (cl_mem)edges.get_storage());
        kernel->set_argument(2, p.getBuffer() );
        kernel->set_argument(3, normals.getBuffer());

        kernel->set_argument(4, resampled_points.getBuffer());
        kernel->set_argument(5, stops.getBuffer());

        ctx->launch1d(kernel.get(), p.size());
        ctx->synchronize();

        return std::make_tuple(std::move(resampled_points), std::move(stops));

    }


    inline points deform_average_samples(freeform::context* ctx, const points& p)
    {
        points avg_points(ctx->get_control());


        avg_points.resize(p.size());

        auto s = p.size() / 4;

        auto program = create_freeform_deform_average_samples_kernel(ctx->get_context());
        auto kernel = program->create_kernel("kernel_main");

        kernel->set_argument(0, p.getBuffer());
        kernel->set_argument(1, avg_points.getBuffer());
        kernel->set_argument(2, (uint32_t) s);

        ctx->launch1d(kernel.get(), s);
        ctx->synchronize();

        return std::move(avg_points);

    }

    inline patches deform_gather_samples(freeform::context* ctx, const points& points, const patches& patches_in)
    {
        patches p(ctx->get_control());

        auto s = patches_in.size();
        p.resize(s);

        auto program = create_freeform_deform_gather_samples_kernel(ctx->get_context());
        auto kernel = program->create_kernel("kernel_main");

        kernel->set_argument(0, points.getBuffer());
        kernel->set_argument(1, patches_in.getBuffer());
        kernel->set_argument(2, p.getBuffer());

        ctx->launch1d(kernel.get(), s);
        ctx->synchronize();

        return std::move(p);

    }

    inline std::tuple< patches, stops> deform(freeform::context* ctx, const patches& p, const imaging::opencl_texture& edges)
    {
        stops    stop(ctx->get_control());

        //get normals that we want to transfer along
        auto    s = deform_normal_curve_points(ctx, p);
        auto    normal_vectors      = deform_scatter_normals(ctx, s);
        auto    pts                 = deform_scatter_points(ctx, p);
        auto    deformed            = deform_deform_points(ctx, pts, normal_vectors, edges);
        auto    averaged            = deform_average_samples(ctx, std::get<0>(deformed));
        auto    deformed_patches    = deform_gather_samples(ctx, averaged, p);

    
        return std::make_tuple(std::move(deformed_patches), std::move(stop));
    }
}
