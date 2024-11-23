#pragma once
#include "Quad.h"
#include "Hittable.h"
#include <glm/vec3.hpp>

class Quad : public Hittable
{
public:
	glm::vec3 q;
	glm::vec3 u, v;
	glm::vec3 normal;
	glm::vec3 w;
	float d;
	std::shared_ptr<Material> material;
	AABB bbox;

	Quad(const glm::vec3& q, const glm::vec3& u, const glm::vec3& v, std::shared_ptr<Material> mat) :
		q(q), u(u), v(v), material(mat)
	{
		glm::vec3 n = glm::cross(u, v);
		normal = glm::normalize(n);
		d = glm::dot(normal, q);
		w = n / glm::dot(n, n);
		

		ComputeBoundingBox();
	}

	virtual void ComputeBoundingBox() {
		auto bbox_diagonal1 = AABB(q, q + u + v);
		auto bbox_diagonal2 = AABB(q + u, q + v);
		bbox = AABB(bbox_diagonal1, bbox_diagonal2);
	}

	virtual bool IsInterior(float a, float b, HitRecord& rec) const {
		Interval unit_interval = Interval(0, 1);
		if (!unit_interval.Contains(a) || !unit_interval.Contains(b)) {
			return false;
		}
		rec.u = a;
		rec.v = b;
		return true;
	}

	// Inherited via Hittable
	bool Hit(const Ray& r, Interval ray_t, HitRecord& rec) const override;
	AABB BBox() const override;
};

