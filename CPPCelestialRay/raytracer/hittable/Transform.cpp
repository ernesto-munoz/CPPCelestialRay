#include "Transform.h"



//bool Transform::Hit(const Ray& r, Interval ray_t, HitRecord& rec) const
//{
//	// move the ray in the direcion of the translation
//	glm::uvec4 transformed_origin = transform * glm::vec4(r.origin, 1.0f);
//	Ray offset_ray(transformed_origin, r.direction);
//
//	// determine if the ray intersects
//	if (!object->Hit(offset_ray, ray_t, rec)) {
//		return false;
//	}
//
//	rec.point = glm::inverse(transform) * rec.point;
//	return true;
//}
//
//AABB Transform::BBox() const
//{
//	return bbox;
//}
//
//void Transform::CalculateBoundingBox()
//{
//	bbox = transform * object->BBox();
//}
//
//void Transform::Translate(const glm::vec3 translate)
//{
//	transform = glm::translate(transform, translate);
//}
//
//void Transform::Rotate(float radians, const glm::vec3& axis)
//{
//	transform = glm::rotate(transform, radians, axis);
//}
//
//void Transform::Scale(const glm::vec3& scale)
//{
//	transform = glm::scale(transform, scale);
//}
