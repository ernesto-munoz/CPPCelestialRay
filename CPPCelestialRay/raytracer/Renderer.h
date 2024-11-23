#pragma once
#include <iostream>
#include <format>
#include <random>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/gtc/random.hpp>
#include "Ray.h"
#include "hittable/HittablesList.h"
#include "hittable/Sphere.h"
#include "Camera.h"
#include "ColorBuffer.h"
#include "Resolution.h"
#include "../multithreading/ThreadPool.h"
#include "../utils/RandomUtils.h"
#include "material/Material.h"
#include "accel/BVHNode.h"
#include "material/Texture.h" 
#include "../utils/ScopeTimer.h"
#include "hittable/Quad.h"

using Color = glm::vec3;

struct ThreadingConfig {
	bool use_multithread = true;
	unsigned int patch_width = 64;
	unsigned int patch_height = 64;
	unsigned int num_threads = 10;

	friend std::ostream& operator<<(std::ostream& os, const ThreadingConfig& obj) {
		os << std::format("Threading config ({}): Patch {}x{} Num. threads: {}", obj.use_multithread ? "enabled" : "disabled", obj.patch_width, obj.patch_height, obj.num_threads);
		return os;
	}
};

class RenderConfig {
public:
	Resolution resolution = Resolution(Resolution::kResolution_1280x720);
	size_t max_depth = 20;
	bool is_antialiasing_enabled = true;
	bool is_acceleration_structure_enabled = false;

	void SetSamplesPerPixel(unsigned int spp) {
		samples_per_pixel = spp;
		samples_per_pixel_scale = 1.0f / static_cast<float>(spp);
	}

	unsigned int GetSamplesPerPixel() const {
		return samples_per_pixel;
	}

	float GetSamplesPerPixelScale() const {
		return samples_per_pixel_scale;
	}

	friend std::ostream& operator<<(std::ostream& os, const RenderConfig& obj) {
		os << std::format("Rendering: Resolution {}x{} Antialiasing: {}", obj.resolution.width, obj.resolution.height, obj.GetSamplesPerPixel());
		return os;
	}

private:
	unsigned int samples_per_pixel = 15;
	float samples_per_pixel_scale = 1.0f / samples_per_pixel;


};

class Renderer
{
public:

	// Multithreading
	ThreadingConfig threading_config;
	unsigned int current_render_total_tasks;
	std::mutex current_render_completed_tasks_mutex;
	unsigned int current_render_completed_tasks;

	// Scene
    HittablesList world;
    Camera cam;

	// Rendering
	bool is_rendering = false;
	std::unique_ptr<ColorBuffer> render_buffer = nullptr;
	const glm::vec3 min_allowed_pixel_color = glm::vec3(0.0f, 0.0f, 0.0f);
	const glm::vec3 max_allowed_pixel_color = glm::vec3(1.0f, 1.0f, 1.0f);

	// Configuration
	RenderConfig render_config;

	// Example Scenes
	//void BasicScene();
	//void MultipleSphereScene(const int width = 10);
	//void CheckeredSpheres();
	//void Earth();
	//void Quads();

	Renderer();
	void SetScene(std::function<void(HittablesList&)> create);
	void InitializeRender();
	void FinishRender();
	void ConfigureMultithreading(ThreadingConfig config);
	void ConfigureRender(RenderConfig _render_config);
	void Render();
	void RenderMultiprocessing();
	void RenderPatch(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
	void RenderSingleProcessing();
	glm::vec3 RayColor(const Ray& r, const Hittable& world, size_t depth);
	void SaveRenderBuffer(const std::string outputImagePath) const;
	

private:
	std::unique_ptr<ThreadPool> render_thread_pool;

	void SetColor(unsigned int x, unsigned int y, glm::vec3 color);
	Ray GetRay(unsigned int x, unsigned int y, bool center_ray = false) const;
	std::vector<Ray> GetRays(unsigned int x, unsigned int y, size_t rays_num) const;
	glm::vec3 HemisphericalRand(const glm::vec3& normal);


};

