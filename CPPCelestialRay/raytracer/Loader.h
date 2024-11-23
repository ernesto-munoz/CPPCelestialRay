#pragma once
#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "hittable/Mesh.h"

class Loader
{
public:
	static std::vector<std::shared_ptr<Mesh>> Load(std::string filename);
	static void ProcessNode(aiNode* node, const aiScene* scene, std::vector<std::shared_ptr<Mesh>>& meshes);
	static void ProcessMesh(aiMesh* mesh, const aiScene* scene, std::vector<std::shared_ptr<Mesh>>& meshes);
};

