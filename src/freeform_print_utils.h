#pragma once

#include <iostream>
#include <algorithm>

#include "opencl/opencl_freeform_struct.h"

namespace freeform
{
    template <typename container > inline void print(const container& c)
    {
        for (auto i = 0U; i < c.size(); ++i)
        {
            std::cout << c[i] << " ";
        }
    }

    inline std::ostream& operator<<(std::ostream& s, const freeform_patch& p)
    {
        s << "x: " << p.x0 << " " << p.x1 << " " << p.x2 << " " << p.x3 << std::endl;
        s << "y: " << p.y0 << " " << p.y1 << " " << p.y2 << " " << p.y3 << std::endl;
        return s;
    }

}