#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Hittable.h"

struct Face {
	std::vector<unsigned int> indices;
	glm::vec3 normal;

	Face(std::vector<unsigned int> indices, glm::vec3 normal) : indices(indices), normal(normal) {}
};

class Mesh : public Hittable
{
public:
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	//std::vector<glm::vec3> texture_coords;
	std::vector<Face> faces;

	void SetMaterial(std::shared_ptr<Material> mat);

	// Inherited via Hittable
	bool Hit(const Ray& r, Interval ray_t, HitRecord& rec) const override;
	AABB BBox() const override;
	void CalculateBBox();


private:
	std::shared_ptr<Material> material;
	AABB bbox;
};

