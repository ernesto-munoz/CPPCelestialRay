#pragma once
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include "Ray.h"
//#include "Interval.h"
//#include "AABB.h"
//#include "Hittable.h"


//class Transform : public Hittable
//{
//public:
//	Transform(std::shared_ptr<Hittable> object) : object(object) {
//		CalculateBoundingBox(); //bbox = object->BBox() + offset;
//	}
//	// Inherited via Hittable
//	bool Hit(const Ray& r, Interval ray_t, HitRecord& rec) const override;
//	AABB BBox() const override;
//
//private:
//	std::shared_ptr<Hittable> object;
//	glm::mat4 transform = glm::mat4(1.0f);  // identity matrix
//	AABB bbox;
//
//	void CalculateBoundingBox();
//	void Translate(const glm::vec3 translate);
//	void Rotate(float radians, const glm::vec3& axis);
//	void Scale(const glm::vec3& scale);
//};