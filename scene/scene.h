#pragma once

#include <vector>
#include <memory>
#include "entity.h"
#include "light_manager.h"
#include "camera.h"
#include "rendering/renderer.h"

class Scene
{
public:
    Scene() = default;
    ~Scene() = default;

    /*
    AddEntity(const Entity& entity)
    같은 메서드에서 entities.push_back(entity); 하면 값 복사로 들어갑니다.
    */
    void AddEntity(const std::shared_ptr<Entity>& entity);
    void RemoveEntity(const std::shared_ptr<Entity>& entity);
    void ClearEntities();
    const std::vector<std::shared_ptr<Entity>>& GetEntities() const;
    Light_Manager& GetLightManager();
    void SetCamera(const std::shared_ptr<Camera>& camera);
    const std::shared_ptr<Camera>& GetCamera() const;
    void Update(float delta_time);
    void Render(Renderer& renderer);

private:
    std::vector<std::shared_ptr<Entity>> entities;
    Light_Manager light_manager;
    std::shared_ptr<Camera> camera;
};