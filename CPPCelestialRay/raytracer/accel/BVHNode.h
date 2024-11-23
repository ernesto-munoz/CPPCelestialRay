#pragma once
#include <iostream>
#include <algorithm>
#include "../hittable/Hittable.h"
#include "../hittable/HittablesList.h"
#include "../accel/AABB.h"
#include "../Resolution.h"

class BVHNode : public Hittable
{
public:
	std::shared_ptr<Hittable> left;
	std::shared_ptr<Hittable> right;
	AABB bbox;

	BVHNode(HittablesList list) : BVHNode(list.objects, 0, list.objects.size()) {}
	BVHNode(std::vector<std::shared_ptr<Hittable>>& objects, size_t start, size_t end);
	

	// Inherited via Hittable
	bool Hit(const Ray& r, Interval ray_t, HitRecord& rec) const override;
	AABB BBox() const override;

private:
	static bool BoxCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b, int axis);
	static bool BoxCompareX(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b);
	static bool BoxCompareY(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b);
	static bool BoxCompareZ(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b);

};

