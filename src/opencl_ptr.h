#pragma once

#include <boost/intrusive_ptr.hpp>

#include <memory>

#include "opencl_device.h"

namespace opencl
{
    typedef std::shared_ptr<device>  device_ptr;
}