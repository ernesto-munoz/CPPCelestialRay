#include "Material.h"

bool Lambert::Scatter(const Ray& ray_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const
{
	auto scatter_direction = rec.normal + glm::sphericalRand(1.0f);

	if (NearZero(scatter_direction)) {
		scatter_direction = rec.normal;
	}

	scattered = Ray(rec.point, glm::normalize(scatter_direction));
	attenuation = texture->GetColor(rec.u, rec.v, rec.point);

	return true;
}

bool Metal::Scatter(const Ray& ray_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const
{
	// get the reflection of the incident ray (normalized) and add a little fuzz
	glm::vec3 reflected = Reflect(ray_in.direction, rec.normal) + (glm::sphericalRand(1.0f) * fuzz);
	scattered = Ray(rec.point, reflected);
	attenuation = albedo;
	return glm::dot(scattered.direction, rec.normal) > 0;
}

bool Dielectric::Scatter(const Ray& ray_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const
{
	attenuation = albedo;
	float ri = rec.front_face ? (1.0f / ior) : ior;
	glm::vec3 unit_direction = glm::normalize(ray_in.direction);

	float cos_theta = std::fmin(glm::dot(-unit_direction, rec.normal), 1.0);
	float sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);
	bool cannot_refract = ri * sin_theta > 1.0;
	glm::vec3 direction;
	if (cannot_refract || Reflectance(cos_theta, ri) > rand01()) {
		direction = Reflect(unit_direction, rec.normal);
	}
	else {
		direction = Refract(unit_direction, rec.normal, ri);
	}
	
	scattered = Ray(rec.point, direction);
	return true;
}
