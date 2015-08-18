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

    inline std::tuple< patches , bolt::cl::device_vector<uint32_t> > converged( freeform::context* ctx, const patches& p)
    {
        bolt::cl::device_vector<uint32_t> stop(ctx->get_control());
        patches                           r(ctx->get_control());


    
        return std::make_tuple( std::move(r), std::move(stop) );
    }
}
