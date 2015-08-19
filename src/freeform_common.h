#pragma once

#include <bolt/cl/device_vector.h>

#include "opencl/opencl_freeform_struct.h"

namespace freeform
{
    typedef bolt::cl::device_vector< freeform_patch >          patches;
    typedef bolt::cl::device_vector< freeform_point >          points;
    typedef bolt::cl::device_vector< freeform_sample >         samples;
    typedef bolt::cl::device_vector< uint32_t >                stops;
}
