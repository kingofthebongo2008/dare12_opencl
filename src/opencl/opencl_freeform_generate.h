#include "opencl_stdint.h"
#include "opencl_freeform_generate_struct.h"

float sample_x(uint32_t i, uint32_t step, const generate_sample* s)
{
    return s->m_center_x + s->m_radius * cos((i + step) * s->m_step);
}

float sample_y(uint32_t i, uint32_t step, const generate_sample* s)
{
    return s->m_center_y + s->m_radius * sin((i + step) * s->m_step);
}

freeform_sample sample(uint32_t i, const generate_sample* s)
{
    float x0 = sample_x(3 * i, 0, s);
    float x1 = sample_x(3 * i, 1, s);
    float x2 = sample_x(3 * i, 2, s);
    float x3 = sample_x(3 * i, 3, s);

    float y0 = sample_y(3 * i, 0, s);
    float y1 = sample_y(3 * i, 1, s);
    float y2 = sample_y(3 * i, 2, s);
    float y3 = sample_y(3 * i, 3, s);

    freeform_sample p0 = { x0, x1, x2, x3, y0, y1, y2, y3 };

    return p0;
}



