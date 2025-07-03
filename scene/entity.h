#include "geometry/primitive.h"
#include "material/material.h"

class Entity 
{
public:
    std::vector<Mesh> parts;
    Material material;
    Transform transform;
    
    void Update(float delta);

    void ComputeLocalAABB();
    const AABB& GetLocalAABB();

private:
    AABB local_aabb;
};