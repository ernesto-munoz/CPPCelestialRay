#include "HittablesList.h"

void HittablesList::Clear() {
	objects.clear();
}

void HittablesList::Add(std::shared_ptr<Hittable> object) {
	objects.emplace_back(object);
	bbox = AABB(bbox, object->BBox());
}

bool HittablesList::Hit(const Ray& r, Interval ray_t, HitRecord& rec) const {
	HitRecord tempRec;
	bool hitted = false;
	double closestDistance = ray_t.max;

	for (const auto& object : objects) {
		if (object->Hit(r, Interval(ray_t.min, closestDistance), tempRec)) {
			hitted = true;
			closestDistance = tempRec.t;
			rec = tempRec;
		}
	}

	return hitted;
}

AABB HittablesList::BBox() const
{
	return bbox;
}
