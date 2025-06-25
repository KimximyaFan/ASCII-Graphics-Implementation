#include "geometry/primitive.h"
#include "material/material.h"

class Entity 
{
public:
    std::vector<Mesh> parts;
    Material material;
    Transform   transform;
    
    /*
    void draw(Renderer& r) const {
        // 오브젝트 전체 트랜스폼 설정
        r.setTransform(transform.toMatrix());

        // 각 서브메시를 순회하며 그리기
        for (auto& part : parts) {
            r.setMaterial(part.material);
            part.mesh->draw(r);
        }
    }
        */
    void Update(float delta) {};
};