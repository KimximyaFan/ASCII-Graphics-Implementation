#pragma once

#include "material/color.h"

struct Projected_Vertex
{
    float x, y, z, invW;
    Color color;
};