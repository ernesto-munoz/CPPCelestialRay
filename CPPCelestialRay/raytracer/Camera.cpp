#include "Camera.h"

void Camera::Initialize() {
	aspect_ratio = image_width / image_height;

	// determine viewport dimensions
	float theta = glm::radians(vfov);
	float h = glm::tan(theta * 0.5f);
	float viewport_height = 2 * h * focus_distance;
	float viewport_width = viewport_height * (static_cast<float>(image_width) / image_height);

	// calculate the u, v, w unit basis vectors for the camera coordinate frame
	w = glm::normalize(center - lookat);
	u = glm::normalize(glm::cross(vup, w));
	v = glm::cross(w, u);

	// Calculate the vectors across the horizontal and down the vertical viewport edges.
	auto viewport_u = viewport_width * u;
	auto viewport_v = viewport_height * -v;

	// Calculate the horizontal and vertical delta vectors from pixel to pixel.
	pixel_delta_u = viewport_u / static_cast<float>(image_width);
	pixel_delta_v = viewport_v / static_cast<float>(image_height);

	// Calculate the location of the upper left pixel.
	auto viewport_upper_left = center - (focus_distance * w) - viewport_u * 0.5f - viewport_v * 0.5f;
	pixel00_loc = viewport_upper_left + 0.5f * (pixel_delta_u + pixel_delta_v);

	// calculate the camera defocus disk basis vectors
	float defocus_radius = focus_distance * glm::tan(glm::radians(defocus_angle * 0.5f));
	defocus_disk_u = u * defocus_radius;
	defocus_disk_v = v * defocus_radius;
}

void Camera::SetCameraResolution(Resolution res) {
	image_width = res.width;
	image_height = res.height;
}

std::ostream& operator<<(std::ostream& os, const Camera& obj)
{
	os << std::format("Camera: Position({}, {}, {}) LookAt({}, {}, {}) Up({}, {}, {}) VFov({}) DoF({}) FDistance({})",
		obj.center.x, obj.center.y, obj.center.z,
		obj.lookat.x, obj.lookat.y, obj.lookat.z,
		obj.vup.x, obj.vup.y, obj.vup.z,
		obj.vfov, obj.defocus_angle, obj.focus_distance
	);
	return os;
}
