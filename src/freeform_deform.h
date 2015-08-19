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

    inline std::tuple< patches , bolt::cl::device_vector<uint32_t> > deform( freeform::context* ctx, const patches& p)
    {
        bolt::cl::device_vector<uint32_t> stop(ctx->get_control());
        patches                           r(ctx->get_control());


        //get normals that we want to transfer along
        auto    s = deform_normal_curve_points(ctx, p);
        auto    normal_vectors = deform_scatter_normals(ctx, s);




    
        return std::make_tuple( std::move(r), std::move(stop) );
    }
}
