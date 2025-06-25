#pragma once

#include "light.h"
#include <vector>
#include <memory>

class Light_Manager
{
public:
    Light_Manager() = default;
    ~Light_Manager() = default;

    void AddLight(const std::shared_ptr<Light>& light);
    void RemoveLight(const std::shared_ptr<Light>& light);
    void RemoveLightById(int id);
    void ClearLights();
    const std::vector<std::shared_ptr<Light>>& GetLights() const;
    void UpdateLights(float delta_time);
private:
    std::vector<std::shared_ptr<Light>> lights;
};