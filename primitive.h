#pragma once

#include "vector.h"
#include <vector>
#include <cstdint>

struct Vertex 
{
    Vec4 position;
    Vec3 normal;
    Vec2 uv;
    Vec4 color;
};

struct Transform 
{
    Vec3 position;
    Vec3 rotation;
    Vec3 scale = {1,1,1};
    //Mat4  toMatrix() const;
};

/*
    std::vector<uint32_t>  indices;
    폴리곤을 각각 vertices의 인덱스 3개 로 본다
    인간 입장에서는 난해하지만
    gpu programming을 염두해둔다면 이런 설계가 효율적이다다
*/
class Mesh {
public:
    std::vector<Vertex>    vertices;
    std::vector<uint32_t>  indices;

    /*
    void uploadToGPU();
    void releaseGPU();
    void draw(Renderer& r) const;
    */
/*
private:
    GLuint vbo = 0, ibo = 0;
    */
};



class Object 
{
public:
    std::vector<Mesh> parts;
    Transform   transform;

    //Material    material;
    
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
};
