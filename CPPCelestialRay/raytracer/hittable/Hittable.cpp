#include "Hittable.h"

//bool Translate::Hit(const Ray& r, Interval ray_t, HitRecord& rec) const
//{
//	// move the ray in the direcion of the translation
//	Ray offset_ray(r.origin - offset, r.direction);
//
//	// determine if the ray intersects
//	if (!object->Hit(offset_ray, ray_t, rec)) {
//		return false;
//	}
//
//	rec.point += offset;
//}
//
//AABB Translate::BBox() const
//{
//	return bbox;
//}
