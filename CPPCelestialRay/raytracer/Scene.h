#pragma once
#include <vector>
#include <optional>
#include <string>
#include "hittable/HittablesList.h"
#include "Camera.h"
#include "accel/BVHNode.h"
#include "Loader.h"

class Scene
{
public:
    Scene() {}
    Scene(const std::string& file_path) { FillFromFilePath(file_path); }

    HittablesList world;
    bool has_been_accelerated = false;

    void Accelerate();
    void AddCamera(const Camera& camera);
    const size_t GetNumCameras() const;
    const Camera& GetCurrentCamera() const;
    void SetCurrentCamera(unsigned int index);
    void SetOverrideCamera(const Camera& camera);
    void RemoveOverrideCamera();
    void PrepareForRender(Resolution res);
    void FillFromFilePath(const std::string& file_path);


private:
    std::vector<Camera> cameras;
    unsigned int current_camera_index = 0;
    std::optional<Camera> override_camera;

};

