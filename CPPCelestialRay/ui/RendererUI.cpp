#include "RendererUI.h"

void RendererUI::Initialize() {
    io = &ImGui::GetIO();  // this returns a reference, i need to convert it to pointer
    for (const Resolution::kResolution res : all_resolutions) {
        all_resolution_nice_name.push_back(Resolution::kResolutionsNiceName.at(res));
    }

    for (const TestScenes::kTestScenes scene : all_test_scene) {
        all_test_scenes_nice_name.push_back(TestScenes::kTestScenesList.at(scene).nice_name);
    }

    SetCurrentScene();
}

void RendererUI::NewFrame() {
    ImGui::NewFrame();


    // RENDER APP CONSTRUCTION AND PRESENTATION STARTS HERE
    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        // Manager input

        if (ImGui::IsKeyDown(ImGuiKey_Escape) && renderer.status == Renderer::Status::kProgress) {
            renderer.CancelRender();
        }


        ImGui::Begin("Configuration");                          // Create a window called "Hello, world!" and append into it.
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io->Framerate, io->Framerate);
        ImGui::Text("Last render time % .3f ms", render_duration_ms);

        render_progress = static_cast<float>(renderer.current_render_completed_tasks) / renderer.current_render_total_tasks;
        ImGui::ProgressBar(render_progress, ImVec2(-1.0f, 0.0f), renderer.status == Renderer::Status::kProgress ? nullptr : "Completed");

        if (renderer.status == Renderer::Status::kProgress) ImGui::BeginDisabled();
        
        if (ImGui::Button("Render")) {
            render_config.resolution = all_resolutions[selected_resolution_index];
            render_config.SetSamplesPerPixel(samples_per_pixel);

            renderer.ConfigureRender(render_config);
            renderer.ConfigureMultithreading(threading_config);

            renderer.cam.center = glm::vec3(camera_position[0], camera_position[1], camera_position[2]);
            renderer.cam.lookat = glm::vec3(camera_lookat[0], camera_lookat[1], camera_lookat[2]);
            renderer.cam.vup = glm::vec3(camera_vup[0], camera_vup[1], camera_vup[2]);

            render_start = std::chrono::high_resolution_clock::now();
            previous_render_preview_time = std::chrono::high_resolution_clock::now();
            auto launch_async_render = [this]() -> Renderer::Status { return this->renderer.Render(); };
            render_future = std::async(std::launch::async, launch_async_render);
        }
        
        if (renderer.status == Renderer::Status::kProgress) ImGui::EndDisabled();
        
        if (ImGui::CollapsingHeader("Scene", ImGuiTreeNodeFlags_None)) {
            if (ImGui::Button("Set")) {           
                SetCurrentScene();
            }
            ImGui::SameLine();
            ImGui::Combo(" ", &selected_test_scene, all_test_scenes_nice_name.data(), static_cast<int>(all_test_scenes_nice_name.size()));
        }

        // Camera configuration
        if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_None)) {
            ImGui::InputFloat3("Position", camera_position);
            ImGui::InputFloat3("Look At", camera_lookat);
            ImGui::InputFloat3("Up", camera_vup);
            ImGui::InputFloat("VFov", &renderer.cam.vfov);
            ImGui::InputFloat("Depth of field", &renderer.cam.defocus_angle);
            ImGui::InputFloat("Focus distance", &renderer.cam.focus_distance);
        }        

        // Resolution Combo Box
        if (ImGui::CollapsingHeader("Render configuration", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Combo("Resolution", &selected_resolution_index, all_resolution_nice_name.data(), static_cast<int>(all_resolution_nice_name.size()));
            ImGui::Checkbox("Antialiasing", &render_config.is_antialiasing_enabled);
            ImGui::Checkbox("Acceleration Structure", &render_config.is_acceleration_structure_enabled);

            if (!render_config.is_antialiasing_enabled) ImGui::BeginDisabled();
            ImGui::InputInt("Samples", &samples_per_pixel);
            if (!render_config.is_antialiasing_enabled) ImGui::EndDisabled();
        }
        
        // Multithreading configuration
        if (ImGui::CollapsingHeader("Multithreading configuration", ImGuiTreeNodeFlags_None)) {
            ImGui::Checkbox("Multithreading", &threading_config.use_multithread);
            ImGui::DragScalar("Patch Size ", ImGuiDataType_U16, &threading_config.patch_width, 1.0f, &patch_size_min, &patch_size_max);
            threading_config.patch_height = threading_config.patch_width;
            //ImGui::Combo(num_of_threads_combo_box_text.c_str(), &selected_num_of_threads, all_possible_num_of_threads.data(), all_possible_num_of_threads.size());
            if (ImGui::BeginCombo(num_of_threads_combo_box_text.c_str(), std::to_string(threading_config.num_threads).c_str())) {
                for (unsigned int i = 1; i < std::thread::hardware_concurrency() + 10; ++i) {
                    const bool is_selected = (threading_config.num_threads == i);
                    if (ImGui::Selectable(std::to_string(i).c_str(), is_selected)) {
                        threading_config.num_threads = i;
                    }
                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }

        }

        //ImGui::SameLine();
        if (ImGui::Button("Save")) {
            renderer.SaveRenderBuffer("final_render.png");
        }

        if (renderer.status == Renderer::Status::kProgress || renderer.status_detail == Renderer::StatusDetail::kCancelled) {
            if (std::chrono::high_resolution_clock::now() - previous_render_preview_time  > render_preview_duration_ms) {
                previous_render_preview_time = std::chrono::high_resolution_clock::now();

                DeleteRenderTexture(render_texture_id);
                render_texture_id = CreateRenderTexture(renderer.render_buffer->raw_data, renderer.cam.image_width, renderer.cam.image_height);

            };
        }

        if (render_future.valid()) {
            if (render_future.wait_for(std::chrono::milliseconds(10)) == std::future_status::ready) {
                // get the time to make the render
                render_end = std::chrono::high_resolution_clock::now();
                render_duration = render_end - render_start;
                render_duration_ms = render_duration.count() * 1000.0f;

                // delete the texture and create again with the current render state
                DeleteRenderTexture(render_texture_id);
                render_texture_id = CreateRenderTexture(renderer.render_buffer->raw_data, renderer.cam.image_width, renderer.cam.image_height);

                try {
                    render_future.get(); // This will get the result of the async function
                }
                catch (const std::exception& e) {
                    std::cerr << "Exception: " << e.what() << std::endl; // the promise is broken here if the render is cancelled, i don't know why
                }
            }
        }
        

        if (render_texture_id != NULL) {
            ImGui::GetBackgroundDrawList()->AddImage(
                (ImTextureID)(uintptr_t)render_texture_id,
                ImVec2(0, 0),
                ImVec2(static_cast<float>(renderer.cam.image_width), static_cast<float>(renderer.cam.image_height)));
        }
        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        ImGui::End();
    }
}

GLuint RendererUI::CreateRenderTexture(unsigned char* data, int width, int height)
{
    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    return id;
}

void RendererUI::DeleteRenderTexture(GLuint& texture_id)
{
    // free the memory before reuse the texture id
    if (texture_id != NULL) 
        glDeleteTextures(1, &texture_id); 
}

void RendererUI::SetCurrentScene()
{
    // move test scene cmaera value to the values in the ui
    Camera cam = TestScenes::kTestScenesList.at(all_test_scene[selected_test_scene]).cam;
    std::copy(glm::value_ptr(cam.center), glm::value_ptr(cam.center) + 3, camera_position);
    std::copy(glm::value_ptr(cam.lookat), glm::value_ptr(cam.lookat) + 3, camera_lookat);
    std::copy(glm::value_ptr(cam.vup), glm::value_ptr(cam.vup) + 3, camera_vup);
    renderer.cam.vfov = cam.vfov;
    renderer.cam.defocus_angle = cam.defocus_angle;
    renderer.cam.focus_distance = cam.focus_distance;

    // create the world of the renderer
    renderer.SetScene(TestScenes::kTestScenesList.at(all_test_scene[selected_test_scene]).create);
}


