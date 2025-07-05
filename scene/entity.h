#pragma once

#include "geometry/primitive.h"
#include "material/material.h"
#include "math/matrix.h"

class Entity 
{
public:
    std::vector<Mesh> parts;
    Material material;
    Transform transform;
    
    void Update(float delta);

    void ComputeLocalAABB();
    const AABB& GetLocalAABB();
    Mat4x4 GetLocalToWorldMatrix() const;

private:
    AABB local_aabb;
};