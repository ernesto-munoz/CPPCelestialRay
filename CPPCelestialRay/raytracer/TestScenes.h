#pragma once
#include <map>
#include <vector>
#include <functional>
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
#include "Loader.h"
#include "Scene.h"

//void MultipleSphereScene(HittablesList& world);
const Scene MultipleSphereScene();
const Scene BasicScene();
const Scene CheckeredSpheres();
const Scene Earth();
const Scene Earth4K();
const Scene Quads();
const Scene CornellBox();
inline std::shared_ptr<Hittable> box(const glm::vec3& a, const glm::vec3& b, std::shared_ptr<Material> mat);
const Scene AssimpCubeLoader();
const Scene SimpleLight();

struct TestSceneInfo {
	const char* nice_name;
	std::function<const Scene()> create;
};

class TestScenes
{
public :
	
	enum kTestScenes {
		kTestScenes_MultipleSpheres,
		kTestScenes_BasicScene,
		kTestScenes_CheckeredSpheres,
		kTestScenes_Earth,
		kTestScenes_Earth4K,
		kTestScenes_Quads,
		kTestScenes_CornellBox,
		kTestScenes_AssimpCubeLoader,
		kTestScenes_SimpleLight
	};

	static constexpr std::vector<kTestScenes> GetAllTestScenes() {
		return { kTestScenes_MultipleSpheres, kTestScenes_BasicScene, kTestScenes_CheckeredSpheres, kTestScenes_Earth, kTestScenes_Earth4K, kTestScenes_Quads, kTestScenes_CornellBox, kTestScenes_AssimpCubeLoader, kTestScenes_SimpleLight };
	}

	static const std::map<kTestScenes, TestSceneInfo> kTestScenesList;

};

