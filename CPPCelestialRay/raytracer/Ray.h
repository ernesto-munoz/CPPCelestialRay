#pragma once
#include <iostream>
#include <glm/vec3.hpp>


class Ray
{
public:
	glm::vec3 origin;
	glm::vec3 direction;

	Ray() : origin(glm::vec3(0.0f, 0.0f, 0.0f)), direction(glm::vec3(0.0f, 1.0f, 0.0f)) {}
	Ray(const glm::vec3& origin, const glm::vec3& direction) : origin(origin), direction(direction) {}

	glm::vec3 at(float t) const {
		return origin + t * direction;
	}
	
};

