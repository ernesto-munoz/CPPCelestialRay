#include "Renderer.h"

Renderer::Renderer() {
	render_thread_pool = std::make_unique<ThreadPool>(threading_config.num_threads);
}

void Renderer::SetScene(std::function<void(HittablesList&)> create)
{
	if (create) {
		create(world);
		if (render_config.is_acceleration_structure_enabled)
			world = HittablesList(std::make_shared<BVHNode>(world));
	}
}

void Renderer::InitializeRender() {
	is_rendering = true;
	cam.SetCameraResolution(render_config.resolution);
	cam.Initialize();
	render_buffer = std::make_unique<ColorBuffer>(render_config.resolution.width, render_config.resolution.height);

	current_render_total_tasks = 0;
	current_render_completed_tasks = 0;
}

void Renderer::FinishRender()
{
	is_rendering = false;
}

void Renderer::ConfigureMultithreading(ThreadingConfig config) {
	if (config.num_threads != threading_config.num_threads) {
		render_thread_pool = std::make_unique<ThreadPool>(config.num_threads);
	}
	threading_config = config;
}

void Renderer::ConfigureRender(RenderConfig _render_config) {
	render_config = _render_config;
}

void Renderer::Render() {

	// Initialize render
	InitializeRender();

	// Print renderer condiguration
	std::cout << threading_config << std::endl;
	std::cout << render_config << std::endl;

	// Launche render in multithread or not
	if (!threading_config.use_multithread) { RenderSingleProcessing(); }
	else { RenderMultiprocessing(); }

	FinishRender();
	std::clog << "\rDone.                 \n";
}

void Renderer::RenderMultiprocessing()
{
	std::vector<std::future<void>> results;
	
	for (unsigned int y = 0; y < render_config.resolution.height; y += threading_config.patch_height) {
		for (unsigned int x = 0; x < render_config.resolution.width; x += threading_config.patch_width) {
			unsigned int x_min = x;
			unsigned int x_max = std::min(x + threading_config.patch_width, render_config.resolution.width); // no inclusive
			unsigned int y_min = y;
			unsigned int y_max = std::min(y + threading_config.patch_height, render_config.resolution.height); // no inclusive

			auto task = [this](unsigned int x, unsigned int y, unsigned int w, unsigned int h) -> void {
				RenderPatch(x, y, w, h); 
				current_render_completed_tasks_mutex.lock();
				current_render_completed_tasks++;
				current_render_completed_tasks_mutex.unlock();
			};
			results.emplace_back(
				render_thread_pool->enqueue(task, x, y, threading_config.patch_width, threading_config.patch_height)
			);
			current_render_total_tasks++;
		}
	}
	
	for (auto& result : results) {
		result.get();
	}
}

void Renderer::RenderPatch(unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
	unsigned int x_min = x;
	unsigned int x_max = std::min(x + width, render_config.resolution.width); // no inclusive
	unsigned int y_min = y;
	unsigned int y_max = std::min(y + height, render_config.resolution.height); // no inclusive

	size_t samples_num = render_config.is_antialiasing_enabled ? render_config.GetSamplesPerPixel() : 1;
	float samples_num_ratio = render_config.is_antialiasing_enabled ? render_config.GetSamplesPerPixelScale() : 1;

	for (unsigned int j = y_min; j < y_max; ++j) {
		for (unsigned int i = x_min; i < x_max; ++i) {
			glm::vec3 final_color(0.0f, 0.0f, 0.0f);
			std::vector<Ray> rays = GetRays(i, j, samples_num); // get all the rays needed (the first is centered in the pixel)

			for (int i = 0; i < rays.size(); ++i) {
				final_color += RayColor(rays.at(i), world, render_config.max_depth);
			}

			SetColor(i, j, final_color * samples_num_ratio);
		}
	}
}

void Renderer::RenderSingleProcessing() {
	size_t samples_num = render_config.is_antialiasing_enabled ? render_config.GetSamplesPerPixel() : 1;
	float samples_num_ratio = render_config.is_antialiasing_enabled ? render_config.GetSamplesPerPixelScale() : 1;

	for (unsigned int j = 0; j < render_config.resolution.height; ++j) {
		for (unsigned int i = 0; i < render_config.resolution.width; ++i) {
			glm::vec3 final_color(0.0f, 0.0f, 0.0f);
			std::vector<Ray> rays = GetRays(i, j, samples_num); // get all the rays needed (the first is centered in the pixel)

			for (int i = 0; i < rays.size(); ++i) {
				final_color += RayColor(rays.at(i), world, render_config.max_depth);
			}

			SetColor(i, j, final_color * samples_num_ratio);
		}
	}
}

glm::vec3 Renderer::RayColor(const Ray& r, const Hittable& world, size_t depth) {
	if (depth <= 0) {
		return glm::vec3(0.0f, 0.0f, 0.0f);
	}

	HitRecord rec;
	if (world.Hit(r, Interval(0.0001, INFINITY), rec)) {
		Ray scattered;
		glm::vec3 attenuation;
		if (rec.material->Scatter(r, rec, attenuation, scattered)) {
			return attenuation * RayColor(scattered, world, depth - 1);
		}

		return glm::vec3(0.0f, 0.0f, 0.0f);
	}

	// calculate background color
	glm::vec3 unit_direction = glm::normalize(r.direction);
	float a = 0.5f * (unit_direction.y + 1.0f);
	return (1.0f - a) * glm::vec3(1.0f, 1.0f, 1.0f) + a * glm::vec3(0.5f, 0.7f, 1.0f);
}

void Renderer::SaveRenderBuffer(const std::string outputImagePath) const {
	render_buffer->Save(outputImagePath);
}

void Renderer::SetColor(unsigned int x, unsigned int y, glm::vec3 color) {
	// write the color in the render buffer (should i use a lock here?)

	auto linear_to_gamma = [](float x) {
		if (x > 0) return std::sqrtf(x);
		return 0.0f;
	};

	color.r = linear_to_gamma(color.r);
	color.g = linear_to_gamma(color.g);
	color.b = linear_to_gamma(color.b);

	color = glm::clamp(color, min_allowed_pixel_color, max_allowed_pixel_color);
	render_buffer->raw_data[(y * render_config.resolution.width + x) * 3] = static_cast<unsigned char>(color.r * 255);
	render_buffer->raw_data[(y * render_config.resolution.width + x) * 3 + 1] = static_cast<unsigned char>(color.g * 255);
	render_buffer->raw_data[(y * render_config.resolution.width + x) * 3 + 2] = static_cast<unsigned char>(color.b * 255);
}

Ray Renderer::GetRay(unsigned int x, unsigned int y, bool center_ray) const {
	// Construct a camera ray origination from the deficus disk and directed at randomly sampled point around the pixel location (x, y)

	// Square sample: change in the future for something better
	glm::vec2 offset = glm::vec2(0.0f, 0.0f);
	if (!center_ray) {
		offset.x = rand01() - 0.5f;
		offset.y = rand01() - 0.5f;
	}

	auto pixel_sample = cam.pixel00_loc + ((static_cast<float>(x) + offset.x) * cam.pixel_delta_u) + ((static_cast<float>(y) + offset.y) * cam.pixel_delta_v);
	auto random_unit_disk = glm::diskRand(1.0f);
	/*std::cout << random_unit_disk.x << " " << random_unit_disk.y << std::endl;*/
	// if no defocus angle, from the center; if defocus angle from a random desviation of length 1 from the center
	auto ray_origin = (cam.defocus_angle <= 0) ? cam.center : cam.center + (random_unit_disk.x * cam.defocus_disk_u) + (random_unit_disk.y * cam.defocus_disk_v);
	auto ray_direction = pixel_sample - ray_origin;
	return Ray(ray_origin, ray_direction);
}

std::vector<Ray> Renderer::GetRays(unsigned int x, unsigned int y, size_t rays_num) const {
	std::vector<Ray> rays;

	rays.emplace_back(GetRay(x, y, true));  // i want the first to be centered
	for (int sample = 1; sample < rays_num; ++sample) {
		rays.emplace_back(GetRay(x, y));
	}
	return rays;

}

glm::vec3 Renderer::HemisphericalRand(const glm::vec3& normal) {
	// TODO: exists analytic methods to calculate a vector in an hemisphere, change in the future this
	glm::vec3 unit_vector = glm::sphericalRand(1.0f);
	if (glm::dot(unit_vector, normal) > 0.0) {
		return unit_vector;
	}
	return -unit_vector;
}