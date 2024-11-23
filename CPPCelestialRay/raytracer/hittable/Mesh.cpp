#include "Mesh.h"

void Mesh::SetMaterial(std::shared_ptr<Material> mat)
{
    material = mat;
}

bool Mesh::Hit(const Ray& r, Interval ray_t, HitRecord& rec) const
{
    constexpr float epsilon = std::numeric_limits<float>::epsilon();

    for (size_t i = 0; i < faces.size(); ++i) {
        glm::vec3 v0 = vertices[faces[i].indices[0]];
        glm::vec3 v1 = vertices[faces[i].indices[1]];
        glm::vec3 v2 = vertices[faces[i].indices[2]];
        glm::vec3 edge1 = v1 - v0;
        glm::vec3 edge2 = v2 - v0;
        glm::vec3 ray_cross_e2 = glm::cross(r.direction, edge2);
        float det = glm::dot(edge1, ray_cross_e2);

        if (det > -epsilon && det < epsilon) {
            return false;  // determinante is near 0, the ray is parallel
        }

        float inv_det = 1.0f / det;

        glm::vec3 s = r.origin - v0;
        float u = inv_det * glm::dot(s, ray_cross_e2);
        if (( u < 0.0f && std::fabs(u) > epsilon) || (u > 1.0f && std::fabs(u - 1) > epsilon)) {
            continue;  // intersection outside the triangle
        }

        glm::vec3 s_cross_e1 = glm::cross(s, edge1);
        float v = inv_det * glm::dot(r.direction, s_cross_e1);
        if ((v < 0.0f && std::fabs(v) > epsilon) || (u + v > 1.0f && std::fabs(u + v - 1) > epsilon)) {
            continue;  // intersection outside the triangle
        }

        float t = inv_det * glm::dot(edge2, s_cross_e1);
        if (t < epsilon) {
            continue;  // intersection behind the ray origin
        }

        rec.t = t;
        rec.point = r.at(t);
        rec.material = material;
        // calculate normal
        /*faces[i].
        glm::vec3 v0_normal = normals[faces[i].indices[0]];*/

        //rec.SetFaceNormal(r, faces[i].normal);
        rec.normal = faces[i].normal;
        

        return true;
    }

    return false;
    
}

AABB Mesh::BBox() const
{
    return AABB();
}
