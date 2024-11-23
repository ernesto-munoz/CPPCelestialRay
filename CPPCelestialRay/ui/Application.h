#pragma once
#include <iostream>
#include <chrono>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include <glm/vec3.hpp> // glm::vec3
#include "RendererUI.h"

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}


struct ApplicationConfig {
    std::string window_name = "RaytracerDOD Main Window";
    unsigned int width = 1280;
    unsigned int height = 720;
};


class Application
{
public:
    ApplicationConfig application_config;
    GLFWwindow* window;
    RendererUI renderer_ui;

    Application() = default;

	void Run();
};

