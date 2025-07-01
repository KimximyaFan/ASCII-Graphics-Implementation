#pragma once

#include "scene.h"
#include <algorithm>

void Scene::AddEntity(const std::shared_ptr<Entity>& entity)
{
    entities.push_back(entity);
}

void Scene::RemoveEntity(const std::shared_ptr<Entity>& entity)
{   /*
    for (size_t i = 0; i < entities.size(); ++i)
    {
        if ( entities[i] == entity )
        {
            entities.erase(entities.begin() + i);
            break;
        }
    }
    */

    // trendy
    entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
}

void Scene::ClearEntities()
{
    entities.clear();
}

const std::vector<std::shared_ptr<Entity>>& Scene::GetEntities() const
{
    return entities;
}

const std::shared_ptr<Light_Manager>& Scene::GetLightManager()
{
    return light_manager;
}

void Scene::SetCamera(const std::shared_ptr<Camera>& cam)
{
    camera = cam;
}

const std::shared_ptr<Camera>& Scene::GetCamera() const
{
    return camera;
}

void Scene::Update(float delta_time)
{
    for (auto& entity : entities)
        entity->Update(delta_time);

    light_manager->UpdateLights(delta_time);
}