#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <cmath>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>  // glm::normalize
#include <glm/gtx/norm.hpp> // glm::lenght2
#include <glm/gtx/intersect.hpp>
#include "Hittable.h"
#include "../material/Material.h"

class Sphere : public Hittable
{
public:
    Sphere(const glm::vec3& center, float radius, std::shared_ptr<Material> material) : center(center), radius(static_cast<float>(std::fmax(0, radius))), material(material) {
        glm::vec3 rvec = glm::vec3(radius);
        bbox = AABB(center - rvec, center + rvec);
    }

    bool Hit(const Ray& r, Interval ray_t, HitRecord& rec) const override;
    AABB BBox() const override { return bbox; }  // TODO: when he system allows animations, the bbox should care about it

private:
    glm::vec3 center;
    float radius;
    std::shared_ptr<Material> material;
    AABB bbox;

    static void GetUV(const glm::vec3 point, double& u, double& v);
};
