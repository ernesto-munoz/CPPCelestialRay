#include "TestScenes.h"


const std::map<TestScenes::kTestScenes, TestSceneInfo> TestScenes::kTestScenesList = {
	{kTestScenes_MultipleSpheres, TestSceneInfo("Multiple Spheres", MultipleSphereScene)},
	{kTestScenes_BasicScene, TestSceneInfo("Basic Scene", BasicScene)},
	{kTestScenes_CheckeredSpheres, TestSceneInfo("Chechered Spheres", CheckeredSpheres)},
	{kTestScenes_Earth, TestSceneInfo("Earth", Earth)},
	{kTestScenes_Earth4K, TestSceneInfo("Earth 4K", Earth4K)},
	{kTestScenes_Quads, TestSceneInfo("Quads", Quads)},
	{kTestScenes_CornellBox, TestSceneInfo("Cornell Box", CornellBox)},
	{kTestScenes_AssimpCubeLoader, TestSceneInfo("Assimp Cube Loader", AssimpCubeLoader)},
	{ kTestScenes_SimpleLight, TestSceneInfo("Simple Light", SimpleLight) }
};

const Scene MultipleSphereScene()
{
	Scene scene;
	scene.AddCamera(Camera(glm::vec3(13, 2, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), 20, 0.6, 10.0));
	scene.AddCamera(Camera(glm::vec3(-13, 2, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), 20, 0.6, 10.0));
	scene.SetCurrentCamera(0);

	auto checker = std::make_shared<CheckerTexture>(0.32, Color(0.2f, 0.3f, 0.1f), Color(0.9f, 0.9f, 0.9f));
	auto ground_material = std::make_shared<Lambert>(Color(0.0f, 0.5f, 1.0f));
	scene.world.Add(make_shared<Sphere>(glm::vec3(0, -1000, 0), 1000, ground_material));
	const int width = 20;
	for (int a = -width; a < width; a++) {
		for (int b = -width; b < width; b++) {
			auto choose_mat = rand01();
			glm::vec3 center(a + 0.9 * rand01(), 0.2, b + 0.9 * rand01());

			if ((center - glm::vec3(4, 0.2, 0)).length() > 0.9) {
				std::shared_ptr<Material> sphere_material;

				if (choose_mat < 0.8) {
					// diffuse
					auto albedo = glm::vec3(rand01(), rand01(), rand01());
					sphere_material = std::make_shared<Lambert>(albedo);
					scene.world.Add(std::make_shared<Sphere>(center, 0.2, sphere_material));
				}
				else if (choose_mat < 0.95) {
					// metal
					auto albedo = glm::vec3(rand01(), rand01(), rand01());
					auto fuzz = rand(0.0f, 0.5f);
					sphere_material = std::make_shared<Metal>(albedo, fuzz);
					scene.world.Add(std::make_shared<Sphere>(center, 0.2, sphere_material));
				}
				else {
					// glass
					sphere_material = std::make_shared<Dielectric>(1.5);
					scene.world.Add(std::make_shared<Sphere>(center, 0.2, sphere_material));
				}
			}
		}
	}

	auto material1 = std::make_shared<Dielectric>(1.5);
	scene.world.Add(std::make_shared<Sphere>(glm::vec3(0, 1, 0), 1.0, material1));

	auto material2 = std::make_shared<Lambert>(glm::vec3(0.4, 0.2, 0.1));
	scene.world.Add(std::make_shared<Sphere>(glm::vec3(-4, 1, 0), 1.0, material2));

	auto material3 = std::make_shared<Metal>(glm::vec3(0.7, 0.6, 0.5), 0.0);
	scene.world.Add(std::make_shared<Sphere>(glm::vec3(4, 1, 0), 1.0, material3));

	return scene;
}

const Scene BasicScene()
{
	Scene scene;
	scene.AddCamera(Camera(glm::vec3(-2, 2, 1), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0), 90, 10.0, 3.4));

	auto material_ground = std::make_shared<Lambert>(glm::vec3(0.8, 0.8, 0.0));
	auto material_center = std::make_shared<Lambert>(glm::vec3(0.1, 0.2, 0.5));
	auto material_right = std::make_shared<Metal>(glm::vec3(0.8, 0.6, 0.2), 1.0);
	auto material_left = std::make_shared<Dielectric>(glm::vec3(1.0, 1.0, 1.0), 1.50);
	auto material_bubble = std::make_shared<Dielectric>(glm::vec3(1.0, 1.0, 1.0), 1.00 / 1.50);

	scene.world.Add(make_shared<Sphere>(glm::vec3(0.0, -100.5, -1.0), 100.0, material_ground));
	scene.world.Add(make_shared<Sphere>(glm::vec3(0.0, 0.0, -1.2), 0.5, material_center));
	scene.world.Add(make_shared<Sphere>(glm::vec3(-1.0, 0.0, -1.0), 0.5, material_left));
	scene.world.Add(make_shared<Sphere>(glm::vec3(-1.0, 0.0, -1.0), 0.4, material_bubble));
	scene.world.Add(make_shared<Sphere>(glm::vec3(1.0, 0.0, -1.0), 0.5, material_right));

	return scene;
}

const Scene CheckeredSpheres()
{
	Scene scene;
	scene.AddCamera(Camera(glm::vec3(13, 2, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), 20, 0.0, 10.0));
	auto checker = std::make_shared<CheckerTexture>(0.32, Color(.2, .3, .1), Color(.9, .9, .9));

	scene.world.Add(std::make_shared<Sphere>(glm::vec3(0, -10, 0), 10, std::make_shared<Lambert>(checker)));
	scene.world.Add(std::make_shared<Sphere>(glm::vec3(0, 10, 0), 10, std::make_shared<Lambert>(checker)));

	return scene;
}

const Scene Earth()
{
	Scene scene;
	scene.AddCamera(Camera(glm::vec3(0, 0, 12), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), 20, 0.0, 10.0));
	std::shared_ptr<ImageTexture> earth_texture;
	{
		ScopeTimer t("Earth texture time took ");
		earth_texture = std::make_shared<ImageTexture>("../resources/earthmap.jpg");
	}
	auto earth_surface = std::make_shared<Lambert>(earth_texture);
	auto globe = std::make_shared<Sphere>(glm::vec3(0, 0, 0), 2, earth_surface);
	scene.world.Add(globe);

	return scene;
}

const Scene Earth4K()
{
	Scene scene;
	scene.AddCamera(Camera(glm::vec3(0, 0, 12), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), 20, 0.0, 10.0));
	std::shared_ptr<ImageTexture> earth_texture;
	{
		ScopeTimer t("Earth texture time took ");
		earth_texture = std::make_shared<ImageTexture>("../resources/8k_earth_daymap.jpg");
	}
	auto earth_surface = std::make_shared<Lambert>(earth_texture);
	auto globe = std::make_shared<Sphere>(glm::vec3(0, 0, 0), 2, earth_surface);
	scene.world.Add(globe);

	return scene;
}

const Scene Quads()
{
	Scene scene;
	scene.AddCamera(Camera(glm::vec3(0, 0, 9), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), 80, 0, 10));

	// Materials
	auto left_red = std::make_shared<Lambert>(Color(1.0, 0.2, 0.2));
	auto back_green = std::make_shared<Lambert>(Color(0.2, 1.0, 0.2));
	auto right_blue = std::make_shared<Lambert>(Color(0.2, 0.2, 1.0));
	auto upper_orange = std::make_shared<Lambert>(Color(1.0, 0.5, 0.0));
	auto lower_teal = std::make_shared<Lambert>(Color(0.2, 0.8, 0.8));

	// Quads
	scene.world.Add(std::make_shared<Quad>(glm::vec3(-3, -2, 5), glm::vec3(0, 0, -4), glm::vec3(0, 4, 0), left_red));
	scene.world.Add(std::make_shared<Quad>(glm::vec3(-2, -2, 0), glm::vec3(4, 0, 0), glm::vec3(0, 4, 0), back_green));
	scene.world.Add(std::make_shared<Quad>(glm::vec3(3, -2, 1), glm::vec3(0, 0, 4), glm::vec3(0, 4, 0), right_blue));
	scene.world.Add(std::make_shared<Quad>(glm::vec3(-2, 3, 1), glm::vec3(4, 0, 0), glm::vec3(0, 0, 4), upper_orange));
	scene.world.Add(std::make_shared<Quad>(glm::vec3(-2, -3, 5), glm::vec3(4, 0, 0), glm::vec3(0, 0, -4), lower_teal));

	return scene;
}

const Scene CornellBox()
{
	Scene scene;
	scene.AddCamera(Camera(glm::vec3(278, 278, -800), glm::vec3(278, 278, 0), glm::vec3(0, 1, 0), 40, 0, 10));
	// Materials
	auto red = std::make_shared<Lambert>(Color(1.0, 0.2, 0.2));
	auto green = std::make_shared<Lambert>(Color(0.2, 1.0, 0.2));
	auto white = std::make_shared<Lambert>(Color(1.0, 1.0, 1.0));

	scene.world.Add(std::make_shared<Quad>(glm::vec3(555, 0, 0), glm::vec3(0, 555, 0), glm::vec3(0, 0, 555), green));
	scene.world.Add(std::make_shared<Quad>(glm::vec3(0, 0, 0), glm::vec3(0, 555, 0), glm::vec3(0, 0, 555), red));
	scene.world.Add(std::make_shared<Quad>(glm::vec3(343, 554, 332), glm::vec3(-130, 0, 0), glm::vec3(0, 0, -105), white));
	scene.world.Add(std::make_shared<Quad>(glm::vec3(0, 0, 0), glm::vec3(555, 0, 0), glm::vec3(0, 0, 555), white));
	scene.world.Add(std::make_shared<Quad>(glm::vec3(555, 555, 555), glm::vec3(-555, 0, 0), glm::vec3(0, 0, -555), white));
	scene.world.Add(std::make_shared<Quad>(glm::vec3(0, 0, 555), glm::vec3(555, 0, 0), glm::vec3(0, 555, 0), white));

	scene.world.Add(box(glm::vec3(130, 0, 65), glm::vec3(295, 165, 230), white));
	scene.world.Add(box(glm::vec3(265, 0, 295), glm::vec3(430, 330, 460), white));

	return scene;
}

inline std::shared_ptr<Hittable> box(const glm::vec3& a, const glm::vec3& b, std::shared_ptr<Material> mat)
{
	// Returns the 3D box (six sides) that contains the two opposite vertices a & b.

	auto sides = std::make_shared<HittablesList>();

	// Construct the two opposite vertices with the minimum and maximum coordinates.
	auto min = glm::vec3(std::fmin(a.x, b.x), std::fmin(a.y, b.y), std::fmin(a.z, b.z));
	auto max = glm::vec3(std::fmax(a.x, b.x), std::fmax(a.y, b.y), std::fmax(a.z, b.z));

	auto dx = glm::vec3(max.x - min.x, 0, 0);
	auto dy = glm::vec3(0, max.y - min.y, 0);
	auto dz = glm::vec3(0, 0, max.z - min.z);

	sides->Add(make_shared<Quad>(glm::vec3(min.x, min.y, max.z), dx, dy, mat)); // front
	sides->Add(make_shared<Quad>(glm::vec3(max.x, min.y, max.z), -dz, dy, mat)); // right
	sides->Add(make_shared<Quad>(glm::vec3(max.x, min.y, min.z), -dx, dy, mat)); // back
	sides->Add(make_shared<Quad>(glm::vec3(min.x, min.y, min.z), dz, dy, mat)); // left
	sides->Add(make_shared<Quad>(glm::vec3(min.x, max.y, max.z), dx, -dz, mat)); // top
	sides->Add(make_shared<Quad>(glm::vec3(min.x, min.y, min.z), dx, dz, mat)); // bottom

	return sides;
}

const Scene AssimpCubeLoader()
{
	Scene scene;
	//scene.AddCamera(Camera(glm::vec3(0, 2, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), 20, 0, 10));
	
	// Materials
	auto material_ground = std::make_shared<Lambert>(glm::vec3(0.8, 0.8, 0.0));
	auto material_center = std::make_shared<Lambert>(glm::vec3(0.1, 0.2, 0.5));
	auto red = std::make_shared<Lambert>(glm::vec3(1.0, 0.0, 0.0));
	auto green = std::make_shared<Lambert>(glm::vec3(0.0, 1.0, 0.0));
	auto blue = std::make_shared<Lambert>(glm::vec3(0.0, 0.0, 1.0));
	auto material_right = std::make_shared<Metal>(glm::vec3(0.8, 0.6, 0.2), 1.0);
	auto material_left = std::make_shared<Dielectric>(glm::vec3(1.0, 1.0, 1.0), 1.50);
	auto material_bubble = std::make_shared<Dielectric>(glm::vec3(1.0, 1.0, 1.0), 1.00 / 1.50);

	std::vector<std::shared_ptr<Mesh>> meshes;
	Loader::GetMeshes("../resources/monkey.glb", meshes);

	for (auto mesh : meshes) {
		mesh->SetMaterial(material_center);
		scene.world.Add(mesh);
	}

	scene.FillFromFilePath("../resources/monkey.glb");
	
	return scene;
}


const Scene SimpleLight()
{
	Scene scene;
	scene.AddCamera(Camera(glm::vec3(26, 3, 6), glm::vec3(0, 2, 0), glm::vec3(0, 1, 0), 20, 0, 10));

	//auto pertext = std::make_shared<>(4);
	//world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
	//world.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(pertext)));

	//auto difflight = make_shared<diffuse_light>(color(4, 4, 4));
	//world.add(make_shared<quad>(point3(3, 1, -2), vec3(2, 0, 0), vec3(0, 2, 0), difflight));
	return scene;
}
