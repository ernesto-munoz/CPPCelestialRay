#pragma once
#include <glm/vec3.hpp>
#include <glm/glm.hpp> 
#include "../Ray.h"
#include "../Interval.h"
#include "../accel/AABB.h"

class Material;

struct HitRecord {
	glm::vec3 point;
	glm::vec3 normal;
	double u;
	double v;
	double t;
	bool front_face;
	std::shared_ptr<Material> material;

	void SetFaceNormal(const Ray& r, const glm::vec3& out_normal) {
		// Sets the hit record normal vector.
		// NOTE: the parameter `out_normal` is assumed to have unit length.

		front_face = glm::dot(r.direction, out_normal) < 0;
		normal = front_face ? out_normal : -out_normal;
	}
};

class Hittable
{
public:
	virtual ~Hittable() = default;
	virtual bool Hit(const Ray& r, Interval ray_t, HitRecord& rec) const = 0;
	virtual AABB BBox() const = 0;

};

