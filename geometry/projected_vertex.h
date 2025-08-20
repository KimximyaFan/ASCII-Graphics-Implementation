#pragma once

#include "material/color.h"

struct Projected_Vertex
{
    float x, y, z, invW;
    float u, v;
    float z_ndc;
    float z_over_w, r_over_w, g_over_w, b_over_w, u_over_w, v_over_w;
};