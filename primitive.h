#pragma once

#include "vector.h"

class Vertex
{
public:
    Vec4 position;
    Vec3 normal;
    
    Vertex (float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
};