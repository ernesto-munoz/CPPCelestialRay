#pragma once
#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "hittable/Mesh.h"
#include "Camera.h"

class Loader
{
public:
	static void GetMeshes(const std::string& filename, std::vector<std::shared_ptr<Mesh>>& meshes);
	static void ProcessNode(aiNode* node, const aiScene* scene, std::vector<std::shared_ptr<Mesh>>& meshes);
	static void ProcessMesh(aiMesh* mesh, const aiScene* scene, std::vector<std::shared_ptr<Mesh>>& meshes);

	static const aiScene* OpenFile(const std::string& filename);
	static void GetCameras(const std::string& filename, std::vector<Camera>& cameras);
	static Camera FromAssimpCamera(const aiCamera& camera, const aiScene& scene);
};

