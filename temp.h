// renderer.cc
#include "renderer.h"
#include <algorithm>

Renderer::Renderer(int w, int h)
    : width(w), height(h),
      frameBuffer(w*h),
      zBuffer(w*h)
{}

void Renderer::clearBuffers(const Color& clearColor, float clearDepth) {
    std::fill(frameBuffer.begin(), frameBuffer.end(), clearColor);
    std::fill(zBuffer.begin(), zBuffer.end(), clearDepth);
}

Renderer::PVert Renderer::projectVertex(const Vertex& v) {
    // 이미 mvp 곱해진 v.position을 가정
    // NDC → viewport
    float invW = 1.0f / v.position.w;
    float ndcX = v.position.x * invW;
    float ndcY = v.position.y * invW;
    float ndcZ = v.position.z * invW;

    PVert out;
    out.x = (ndcX * 0.5f + 0.5f) * width;
    out.y = (1.0f - (ndcY * 0.5f + 0.5f)) * height; // y 뒤집기
    out.z = ndcZ;
    out.c = { v.color.x, v.color.y, v.color.z, v.color.w };
    return out;
}

void Renderer::drawMesh(const Mesh& mesh, const Mat4& mvp) {
    // 모든 정점에 mvp 적용
    std::vector<Vertex> transformed = mesh.vertices;
    for (auto& v : transformed) {
        v.position = mvp * v.position;
    }

    // 인덱스 3개씩 끊어서 삼각형 그리기
    for (size_t i = 0; i+2 < mesh.indices.size(); i += 3) {
        const Vertex& A = transformed[ mesh.indices[i+0] ];
        const Vertex& B = transformed[ mesh.indices[i+1] ];
        const Vertex& C = transformed[ mesh.indices[i+2] ];
        rasterizeTriangle(A, B, C);
    }
}

void Renderer::rasterizeTriangle(const Vertex& va,
                                 const Vertex& vb,
                                 const Vertex& vc) {
    // 1) 화면 좌표로 변환
    PVert A = projectVertex(va);
    PVert B = projectVertex(vb);
    PVert C = projectVertex(vc);

    // 2) 바운딩 박스 계산
    int minX = std::max(0,   (int)std::floor(std::min({A.x,B.x,C.x})));
    int maxX = std::min(width-1,  (int)std::ceil (std::max({A.x,B.x,C.x})));
    int minY = std::max(0,   (int)std::floor(std::min({A.y,B.y,C.y})));
    int maxY = std::min(height-1, (int)std::ceil (std::max({A.y,B.y,C.y})));

    // 3) 에지 함수 상수
    auto edge = [](const PVert& a, const PVert& b, float x, float y){
        return (b.x - a.x)*(y - a.y) - (b.y - a.y)*(x - a.x);
    };
    float area = edge(A,B,C);

    // 4) 픽셀 순회
    for (int y = minY; y <= maxY; ++y) {
        for (int x = minX; x <= maxX; ++x) {
            // barycentric 좌표
            float w0 = edge(B,C, x+0.5f, y+0.5f);
            float w1 = edge(C,A, x+0.5f, y+0.5f);
            float w2 = edge(A,B, x+0.5f, y+0.5f);
            if (w0>=0 && w1>=0 && w2>=0) {
                w0 /= area; w1 /= area; w2 /= area;
                // 깊이 보간
                float z = w0*A.z + w1*B.z + w2*C.z;
                int idx = y*width + x;
                if (z < zBuffer[idx]) {
                    zBuffer[idx] = z;
                    // 컬러 보간
                    Color col;
                    col.r = w0*A.c.r + w1*B.c.r + w2*C.c.r;
                    col.g = w0*A.c.g + w1*B.c.g + w2*C.c.g;
                    col.b = w0*A.c.b + w1*B.c.b + w2*C.c.b;
                    col.a = w0*A.c.a + w1*B.c.a + w2*C.c.a;
                    frameBuffer[idx] = col;
                }
            }
        }
    }
}

#include "renderer.h"

// 윈도우 크기
const int W = 800, H = 600;
Renderer renderer(W, H);

void renderScene(const std::vector<Object>& scene, const Mat4& viewProj) {
    renderer.clearBuffers({0,0,0,1});
    for (auto& obj : scene) {
        Mat4 mvp = viewProj * obj.transform.toMatrix();
        for (auto& mesh : obj.parts)
            renderer.drawMesh(mesh, mvp);
    }
    // 이제 renderer.getFrameBuffer() 를 픽셀마다 스왑버퍼에 복사하면 끝.
}
