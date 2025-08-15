#include "scene/scene.h"
#include "scene/camera.h"
#include "scene/entity.h"
#include "scene/light_manager.h"
#include "math/vector.h"
#include "light/directional_light.h"
#include "test.h"
#include "light_models/blinn_phong.h"
#include "fps_counter/fps_counter.h"
#include "io/input_handler.h"
#include "io/output_handler.h"
#include "rendering/renderer.h"

int main(int argc, char* argv[])
{
    const int width  = 120;
    const int height = 30;

    Scene scene;

    Vec3 camera_pos = Vec3(5.5f, 5.5f, 5.5f);
    Vec3 camera_target = Vec3(0.0f, 0.0f, 0.0f);
    Vec3 camera_up = Vec3(0.0f, 1.0, 0.0f);
    float fov = 45.0f;
    float cell_aspect = 0.5f;
    float aspect = (float)width / (float)height * cell_aspect;
    float near_plane = -0.1f;
    float far_plane = -50.0f;

    auto camera_ptr = std::make_shared<Camera>(
        camera_pos,
        camera_target,
        camera_up,
        fov,
        aspect,
        near_plane,
        far_plane 
    );
 
    scene.SetCamera(camera_ptr);
 
    scene.GetLightManager()->SetAmbient(Vec3(0.25f, 0.25f, 0.25f));
  
    auto key_light = std::make_shared<Directional_Light>(Vec3(50, 100, 50), 1.0f);

    scene.GetLightManager()->AddLight(key_light);

    std::shared_ptr<Entity> entity;
 
    if ( true )
        entity = CreateCubeEntity_Flat24(5.0f);
    else
        entity = CreateTestCubeEntity(5.0);

    scene.AddEntity(entity);  
 
    Renderer renderer(width, height);
    renderer.SetLightingModel(std::make_unique<Blinn_Phong>());

    Output_Handler output_handler(width, height);

    Fps_Counter fps_counter;
    fps_counter.Start();  
 
    //for (auto& col : renderer.GetFrameBuffer())
        //printf("r=%.2f g=%.2f b=%.2f ", col.r, col.g, col.b);
    
    constexpr float angularSpeed = 60.0f * 3.14159265f / 180.0f;
    auto lastTime = std::chrono::high_resolution_clock::now();
    int fps = 0;
    
    // 라이트 dir 파라미터(바로 아래 라인 중요!)
    //Vec3 light_dir_param = Vec3(100,100,100);   // ← 지금 코드 유지(윗면은 안 맞음)
                                                // 윗면(+Y)도 맞게 하려면 Vec3(-100,-100,-100) 으로 바꿔라.
    /*
    const int degs[12] = {30,60,90,120,150,180,210,240,270,300,330,360};
    for (int k = 0; k < 12; ++k)
    {
        const float deg = (float)degs[k];
        entity->transform.SetRotation(Vec3(0.0f, deg * 3.14159265f / 180.0f, 0.0f));

        renderer.DebugShadeNoRaster_DirectionalOnly(scene, deg, light_dir_param);
        printf("------------------------------------------------------------\n");
    }
    */     
       
     
    while ( Input_Handler::IsSpacePressed() == false )
    {
        auto now   = std::chrono::high_resolution_clock::now();
        float dt   = std::chrono::duration<float>(now - lastTime).count();
        lastTime   = now;

        // 2-b) 엔티티 회전 업데이트 (Y축 기준)
        entity->transform.SetRotation(
            entity->transform.GetRotation() + Vec3(0, angularSpeed * dt, 0)
        );
        
        renderer.Render(scene);
        output_handler.PrintBuffer(renderer.GetFrameBuffer(), fps);
        fps = fps_counter.Get_Fps();
    }
}