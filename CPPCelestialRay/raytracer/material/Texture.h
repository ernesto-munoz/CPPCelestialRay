#pragma once
#include <iostream>
#include <algorithm>
#include <glm/vec3.hpp>
#include "../ColorBuffer.h"

using Color = glm::dvec3;

class Texture
{
public:
	virtual ~Texture() = default;
	virtual Color GetColor(float u, float v, const glm::vec3& point) const = 0;
};


class SolidColor : public Texture {
public:
	Color albedo;

	SolidColor(const Color& albedo) : albedo(albedo) {}
	SolidColor(float red, float green, float blue) : SolidColor(Color(red, green, blue)) {}


	// Inherited via Texture
	Color GetColor(float u, float v, const glm::vec3& point) const override;

};

class CheckerTexture : public Texture {
public:
	float inv_scale;
	std::shared_ptr<Texture> even;
	std::shared_ptr<Texture> odd;

	CheckerTexture(float scale, std::shared_ptr<Texture> even, std::shared_ptr<Texture> odd) :
		inv_scale(1 / scale), even(even), odd(odd) {
	}
	CheckerTexture(float scale, const Color& c1, const Color& c2) :
		inv_scale(1 / scale), even(std::make_shared<SolidColor>(c1)), odd(std::make_shared<SolidColor>(c2)) {}

	// Inherited via Texture
	Color GetColor(float u, float v, const glm::vec3& point) const override;

};

class ImageTexture : public Texture {
public:
	ColorBuffer color_buffer;

	ImageTexture(std::string filename) : color_buffer(filename) {}
	
	// Inherited via Texture
	Color GetColor(float u, float v, const glm::vec3& point) const override;

};