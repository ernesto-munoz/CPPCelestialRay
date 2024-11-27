#pragma once
#include <iostream>
#include <format>
#include <chrono>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
//#include <glm/glm.hpp>  // glm::value_ptr
#include <glm/gtc//type_ptr.hpp>

#include <glm/vec3.hpp> // glm::vec3
#include "../raytracer/Renderer.h"
#include "../raytracer/Resolution.h"
#include "../raytracer/TestScenes.h"

#define MAX_PATH 256

class RendererUI
{
public:
    ImGuiIO* io; // you can't have a reference without value so i need to use a pointer here

    // Our state
    bool show_demo_window = false;
    float render_progress = 1.0f;
    GLuint render_texture_id = NULL;
    unsigned int total_tasks_to_do = 0;


    // Multithreading configuration
    std::string num_of_threads_combo_box_text = std::format("Num. threads: {}", std::thread::hardware_concurrency());
    const unsigned int patch_size_min = 10, patch_size_max = 1024;
    ThreadingConfig threading_config;

    // Render timing
    std::chrono::time_point<std::chrono::steady_clock> render_start, render_end;
    std::chrono::duration<float> render_duration;
    float render_duration_ms = 0.0f;

    // Render show preview timings
    std::chrono::time_point<std::chrono::steady_clock> previous_render_preview_time;
    std::chrono::milliseconds render_preview_duration_ms{ 500 };

    // Camera
    int selected_camera_index = 0;
    bool use_custom_camera = false;
    Camera custom_camera;
    float camera_position[3] = { 0.0f, 0.0f, 9.0f };
    float camera_lookat[3] = { 0.0f, 0.0f, 0.0f };
    float camera_vup[3] = { 0.0f, 1.0f, 0.0f };

    // Render
    Renderer renderer;
    RenderConfig render_config;
    std::future<Renderer::Status> render_future;
    bool is_rendering = false;
    int samples_per_pixel = render_config.GetSamplesPerPixel();

    // Resolutions
    std::vector<Resolution::kResolution> all_resolutions = Resolution::GetAllResolutions();
    std::vector<const char*> all_resolution_nice_name;
    int selected_resolution_index = 0;

    // Test Scenes
    const std::vector < TestScenes::kTestScenes> all_test_scene = TestScenes::GetAllTestScenes();
    std::vector<const char*> all_test_scenes_nice_name;
    int selected_test_scene = 0;

    // Resources loader
    char resources_scene_filepath[MAX_PATH];

    RendererUI() = default;
	void Initialize();
	void NewFrame();
    GLuint CreateRenderTexture(unsigned char* data, int width, int height);
    void DeleteRenderTexture(GLuint& texture_id);
    void SetCurrentScene();

};

