#include "Sphere.h"

bool Sphere::Hit(const Ray& r, Interval ray_t, HitRecord& rec) const {
    glm::vec3 oc = center - r.origin;
    auto a = glm::length2(r.direction);
    auto h = glm::dot(r.direction, oc);
    auto c = glm::length2(oc) - radius * radius;

    auto discriminant = h * h - a * c;
    if (discriminant < 0)
        return false;

    auto sqrtd = std::sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (h - sqrtd) / a;
    if (!ray_t.Surrounds(root)) {
        root = (h + sqrtd) / a;
        if (!ray_t.Surrounds(root)) {
            return false;
        }
    }

    rec.t = root;
    rec.point = r.at(rec.t);
    glm::vec3 out_normal = (rec.point - center) / radius;
    GetUV(out_normal, rec.u, rec.v);
    rec.SetFaceNormal(r, out_normal);
    rec.material = material;

    return true;
}

void Sphere::GetUV(const glm::vec3 point, double& u, double& v)
{
    constexpr double pi = glm::pi<double>();
    auto theta = std::acos(-point.y);
    auto phi = std::atan2(-point.z, -point.x) + pi;
    u = phi / (2 * pi);
    v = theta / pi;
}


