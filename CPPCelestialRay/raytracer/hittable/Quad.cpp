#include "Quad.h"

bool Quad::Hit(const Ray& r, Interval ray_t, HitRecord& rec) const
{
    constexpr float epsilon = std::numeric_limits<float>::epsilon();
    auto denom = glm::dot(normal, r.direction);

    // the ray is parallel to the plane; no hit
    if (std::fabs(denom) < epsilon) {
        return false;
    }

    auto t = (d - glm::dot(normal, r.origin)) / denom;
    if (!ray_t.Contains(t)) {
        return false;
    }

    auto intersection = r.at(t);
    glm::vec3 planar_hitpt_vector = intersection - q;
    float alpha = glm::dot(w, glm::cross(planar_hitpt_vector, v));
    float beta = glm::dot(w, glm::cross(u, planar_hitpt_vector));

    if (!IsInterior(alpha, beta, rec)) {
        return false;
    }

    rec.t = t;
    rec.point = intersection;
    rec.material = material;
    rec.SetFaceNormal(r, normal);
    

    
    return true;
}

AABB Quad::BBox() const
{
    return bbox;
}
