#pragma once
#include "Hittable.h"
#include <memory>
#include <vector>

class HittablesList : public Hittable
{
public:
	std::vector<std::shared_ptr<Hittable>> objects;
	AABB bbox;

	HittablesList() {}
	HittablesList(std::shared_ptr<Hittable> object) { Add(object); }

	void Clear();
	void Add(std::shared_ptr<Hittable> object);
	bool Hit(const Ray& r, Interval ray_t, HitRecord& rec) const override;


	// Inherited via Hittable
	AABB BBox() const override;

};

