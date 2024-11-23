#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtc/random.hpp>
#include <glm/gtx/norm.hpp>
#include "../Ray.h"
#include "../hittable/Hittable.h"
#include "../../utils/RandomUtils.h"
#include "../material/Texture.h"

using Color = glm::vec3;

class Material
{
public:
	virtual ~Material() = default;
	virtual bool Scatter(const Ray& ray_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const = 0;

protected:
	inline bool NearZero(const glm::vec3& v) const {
		auto s = 1e-8;
		return (std::fabs(v[0]) < s) && (std::fabs(v[1]) < s) && (std::fabs(v[2]) < s);
	}

	inline glm::vec3 Reflect(const glm::vec3& v, const glm::vec3& n) const {
		return glm::normalize(v - 2 * glm::dot(v, n) * n);
	}

	inline glm::vec3 Refract(const glm::vec3& uv, const glm::vec3& n, float etai_over_etat) const {
		float cost_theta = static_cast<float>(std::fmin(glm::dot(-uv, n), 1.0));
		glm::vec3 r_out_perpendicular = etai_over_etat * (uv + cost_theta * n);
		glm::vec3 r_out_parallel = -std::sqrt(std::fabs(1.0f - glm::length2(r_out_perpendicular))) * n;
		return r_out_perpendicular + r_out_parallel;
	}

};

class Lambert : public Material {
public:
	std::shared_ptr<Texture> texture;
	
	Lambert(const Color& albedo) : texture(std::make_shared<SolidColor>(albedo)) {}
	Lambert(std::shared_ptr<Texture> texture) : texture(texture) {}
	bool Scatter(const Ray& ray_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const override;

};

class Metal : public Material {
public:
	glm::vec3 albedo;
	float fuzz;

	Metal(const glm::vec3& albedo, float fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}
	bool Scatter(const Ray& ray_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const override;

};

class Dielectric : public Material {
public:
	glm::vec3 albedo;
	float ior;

	Dielectric(float ior) : albedo(glm::vec3(1.0f, 1.0f, 1.0f)), ior(ior) {}
	Dielectric(const glm::vec3& albedo, float ior) : albedo(albedo), ior(ior){}
	bool Scatter(const Ray& ray_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const override;

private:
	inline double Reflectance(double cosine, double refraction_index) const {
		auto r0 = (1 - refraction_index) / (1 + refraction_index);
		r0 = r0 * r0;
		return r0 + (1 - r0) * std::pow((1 - cosine), 5);
	}

};

