#pragma once

#include "vector.h"

struct Transform 
{
    Vec3 position;
    Vec3 rotation;
    Vec3 scale = {1,1,1};
    //Mat4  toMatrix() const;
};