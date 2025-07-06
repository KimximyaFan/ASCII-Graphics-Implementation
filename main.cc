#pragma once

#include "scene/camera.h"
#include "scene/scene.h"
#include "scene/camera.h"
#include "scene/entity.h"
#include "scene/light_manager.h"
#include "math/vector.h"
#include "light/directional_light.h"
#include "test.h"
#include "light models/blinn_phong.h"

int main(int argc, char* argv[])
{
    // 1) 윈도우 크기 설정
    const int width  = 100;
    const int height = 100;

    Scene scene;

    Vec3 camera_pos = Vec3(0.0f, 0.0f, 5.0f);
    Vec3 camera_target = Vec3(0.0f, 0.0f, 0.0f);
    Vec3 camera_up = Vec3(0.0f, 1.0, 0.0f);
    float fov = 60.0f;
    float aspect = width / height;
    float near = 0.1f;
    float far = 100.0f;

    auto camera_ptr = std::make_shared<Camera>(
        camera_pos,
        camera_target,
        camera_up,
        fov,
        aspect,
        near,
        far
    );

    scene.SetCamera(camera_ptr);

    scene.GetLightManager()->SetAmbient(Vec3(0.1f, 0.1f, 0.1f));

    auto key_light = std::make_shared<Directional_Light>(Vec3(1, 1, 1), 0.6f);

    scene.GetLightManager()->AddLight(key_light);

    auto entity = CreateTestCubeEntity(1.0f);

    scene.AddEntity(entity);

    Renderer renderer(width, height);
    renderer.SetLightingModel(std::make_unique<Blinn_Phong>());

    renderer.Render(scene);
}