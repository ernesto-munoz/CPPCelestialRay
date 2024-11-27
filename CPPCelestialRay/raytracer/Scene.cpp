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
	return override_camera.value_or(cameras[current_camera_index]);
}

void Scene::SetCurrentCamera(unsigned int index)
{
	current_camera_index = index;
}

void Scene::SetOverrideCamera(const Camera& camera)
{
	override_camera = camera;
}

void Scene::RemoveOverrideCamera()
{
	override_camera.reset();
}

void Scene::PrepareForRender(Resolution res)
{
	// prepare the camera selected to render
	cameras[current_camera_index].SetCameraResolution(res);
	cameras[current_camera_index].Initialize();
	if (override_camera.has_value()) {
		override_camera.value().SetCameraResolution(res);
		override_camera.value().Initialize();
	}

	

}
