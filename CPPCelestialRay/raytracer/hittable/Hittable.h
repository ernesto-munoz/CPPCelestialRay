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
	float u;
	float v;
	double t;
	bool frontFace;
	std::shared_ptr<Material> material;

	void SetFaceNormal(const Ray& r, const glm::vec3& outNormal) {
		// Sets the hit record normal vector.
		// NOTE: the parameter `outNormal` is assumed to have unit length.

		frontFace = glm::dot(r.direction, outNormal) < 0;
		normal = frontFace ? outNormal : -outNormal;
	}
};

class Hittable
{
public:
	virtual ~Hittable() = default;
	virtual bool Hit(const Ray& r, Interval ray_t, HitRecord& rec) const = 0;
	virtual AABB BBox() const = 0;

};

