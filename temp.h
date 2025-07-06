
// 읽기 전용 getter
    const std::vector<Vertex>& GetVertices() const { return vertices_; }
    const std::vector<uint32_t>& GetIndices() const { return indices_; }
    std::shared_ptr<Material> GetMaterial() const { return material_; }

    // 메시 전체를 한 번에 바꿀 때
    void SetVertices(std::vector<Vertex> verts) {
        vertices_ = std::move(verts);
        ComputeLocalAABB();   // 불변식 자동 유지
    }
    void SetIndices(std::vector<uint32_t> idx) {
        indices_ = std::move(idx);
        // 인덱스만 바뀜에도 AABB 재계산이 필요할 땐 호출
        ComputeLocalAABB();
    }
    void SetMaterial(std::shared_ptr<Material> m) { material_ = std::move(m); }

    // 또는, 개별추가 API
    void AddVertex(const Vertex& v) {
        vertices_.push_back(v);
        // local_aabb = local_aabb.Union(v.position) 같은 incremental 업데이트
    }
    void AddTriangle(uint32_t i0, uint32_t i1, uint32_t i2) {
        indices_.push_back(i0);
        indices_.push_back(i1);
        indices_.push_back(i2);
        // (인덱스만 추가됐으니 AABB는 변함없음)
    }
















int main(int argc, char** argv)
{
    // 1) 윈도우 크기 설정
    const int width  = 800;
    const int height = 600;

    // 2) Scene 생성 및 카메라 설정
    Scene scene;
    auto camera = std::make_shared<Camera>(
        Vec3{0.0f, 1.0f, 5.0f},   // position
        Vec3{0.0f, 0.0f, 0.0f},   // look-at target
        Vec3{0.0f, 1.0f, 0.0f},   // up vector
        60.0f,                    // fovY
        float(width) / height,    // aspect ratio
        0.1f,                     // near plane
        100.0f                    // far plane
    );
    scene.SetCamera(camera);

    // 3) 라이트 매니저 설정
    scene.GetLightManager()->SetAmbient(Vec3{0.1f, 0.1f, 0.1f});
    // 예: PointLight 추가 (PointLight는 Light의 서브클래스라고 가정)
    auto keyLight = std::make_shared<PointLight>(
        Vec3{ 5.0f, 10.0f,  5.0f},   // position
        Vec3{ 1.0f,  1.0f,  1.0f},   // color (white)
        1.0f                         // intensity
    );
    scene.GetLightManager()->AddLight(keyLight);

    // 4) 엔티티 및 메시 추가
    auto entity = std::make_shared<Entity>();
    // 메시와 머티리얼은 LoadMesh, LoadMaterial 같은 유틸 함수로 읽어온다고 가정
    Mesh mesh = LoadMesh("assets/models/teapot.obj");
    auto material = std::make_shared<Material>();
    material->diffuse_color   = Vec3{0.8f, 0.4f, 0.2f};
    material->ambient_coef    = 0.2f;
    material->diffuse_coef    = 1.0f;
    material->specular_coef   = 0.5f;
    material->shininess       = 32.0f;
    mesh.SetMaterial(material);

    entity->AddMesh(mesh);
    scene.AddEntity(entity);

    // 5) Renderer 설정
    Renderer renderer(width, height);
    renderer.SetLightingModel(std::make_unique<Blinn_Phong>());

    // 6) Render 호출
    renderer.Render(scene);

    // 7) 결과를 파일로 저장
    SaveFrameBufferPPM("output.ppm",
                       renderer.GetFrameBuffer(),
                       width, height);

    std::cout << "Rendering complete. Saved to output.ppm\n";
    return 0;
}