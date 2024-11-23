#pragma once
#include <glm/glm.hpp> 
#include <iostream>
#include <limits>
#include "../Interval.h"
#include "../Ray.h"

class AABB
{
public:
	Interval x, y, z;

	AABB() {}  // default aabb is an infinite one
	AABB(const Interval& x, const Interval& y, const Interval& z) : x(x), y(y), z(z) {
		PadMinimum();
	}
	AABB(const glm::vec3& a, const glm::vec3& b) {
		x = (a.x <= b.x) ? Interval(a.x, b.x) : Interval(b.x, a.x);
		y = (a.y <= b.y) ? Interval(a.y, b.y) : Interval(b.y, a.y);
		z = (a.z <= b.z) ? Interval(a.z, b.z) : Interval(b.z, a.z);
		PadMinimum();
	}
	AABB(const AABB& box1, const AABB& box2) {
		x = Interval(box1.x, box2.x);
		y = Interval(box1.y, box2.y);
		z = Interval(box1.z, box2.z);
	}

	const Interval& AxisInterval(int n) const;
	bool Hit(const Ray& ray, Interval ray_t) const;
	unsigned int LongestAxis() const;

	static const AABB empty, universe;


private:
	void PadMinimum();
};

AABB operator+(const AABB& bbox, const glm::vec3& offset);
AABB operator+(const glm::vec3& offset, const AABB& bbox);
AABB operator*(const AABB& bbox, const glm::mat4& matrix);
AABB operator*(const glm::mat4& matrix, const AABB& bbox);