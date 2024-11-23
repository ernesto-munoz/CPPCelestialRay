#pragma once
#include <iostream>
#include <format>
#include "hittable/HittablesList.h"
#include "Resolution.h"



class Camera
{
public:
	double aspect_ratio = 1.0;  // Ratio of image width over height
	int image_width = 100;
	int image_height = 100;
	glm::vec3 center;         // Camera center
	float vfov = 80.0f;
	glm::vec3 lookat = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 vup = glm::vec3(0.0f, 1.0f, 0.0f);
	float defocus_angle = 0.6f;
	float focus_distance = 10.0f;

	Camera() : center(glm::vec3()) {};
	explicit Camera(glm::vec3& center) : center(center) {}
	Camera(const glm::vec3& center, const glm::vec3& lookAt, const glm::vec3& vup, float vfov, float defocus_angle, float focus_distance) : 
		center(center), lookat(lookAt), vup(vup), vfov(vfov), defocus_angle(defocus_angle), focus_distance(focus_distance) {}
	
	void Initialize();
	void SetCameraResolution(Resolution res);

	glm::vec3 pixel00_loc = glm::vec3();    // Location of pixel 0, 0
	glm::vec3 pixel_delta_u = glm::vec3();  // Offset to pixel to the right
	glm::vec3 pixel_delta_v = glm::vec3();  // Offset to pixel below
	glm::vec3 u = glm::vec3();
	glm::vec3 v = glm::vec3();
	glm::vec3 w = glm::vec3();
	glm::vec3 defocus_disk_u = glm::vec3();
	glm::vec3 defocus_disk_v = glm::vec3();

	friend std::ostream& operator<<(std::ostream& os, const Camera& obj);
	
};

