#pragma once
#include <vector>
#include <optional>
#include "hittable/HittablesList.h"
#include "Camera.h"
#include "accel/BVHNode.h"

class Scene
{
public:
    Scene() {}

    HittablesList world;
    bool has_been_accelerated = false;

    void Accelerate();
    void AddCamera(const Camera& camera);
    const Camera& GetCurrentCamera() const;
    void SetCurrentCamera(unsigned int index);
    void SetOverrideCamera(const Camera& camera);
    void RemoveOverrideCamera();
    void PrepareForRender(Resolution res);

private:
    std::vector<Camera> cameras;
    unsigned int current_camera_index = 0;
    std::optional<Camera> override_camera;

};

