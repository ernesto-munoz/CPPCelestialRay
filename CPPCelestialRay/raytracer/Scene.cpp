#include "Scene.h"

void Scene::Accelerate()
{
	if (!has_been_accelerated) {
		world = HittablesList(std::make_shared<BVHNode>(world));
		has_been_accelerated = true;
	}
}

void Scene::AddCamera(const Camera& camera)
{
	cameras.emplace_back(camera);
}

const Camera& Scene::GetCurrentCamera() const
{
	return cameras[current_camera_index];
	// TODO: insert return statement here
}

void Scene::SetCurrentCamera(unsigned int index)
{
	current_camera_index = index;
}

void Scene::PrepareForRender(Resolution res)
{
	// prepare the camera selected to render
	cameras[current_camera_index].SetCameraResolution(res);
	cameras[current_camera_index].Initialize();
}