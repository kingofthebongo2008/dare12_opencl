#pragma once

#include <iostream>
#include <algorithm>

namespace freeform
{
    template <typename container > inline void print(const container& c)
    {
        for (auto i = 0U; i < c.size(); ++i)
        {
            std::cout << c[i] << " ";
        }
    }
}