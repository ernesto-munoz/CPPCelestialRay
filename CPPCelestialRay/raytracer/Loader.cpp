#include "Loader.h"



void Loader::ProcessNode(aiNode* node, const aiScene* scene, std::vector<std::shared_ptr<Mesh>>& meshes)
{
    std::cout << node->mNumMeshes << " " << node->mNumChildren << std::endl;

    // Process each mesh located at the current node
    for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        std::cout << mesh->mName.C_Str() << std::endl;
        ProcessMesh(mesh, scene, meshes);
    }

    // Recursively process each child node
    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        ProcessNode(node->mChildren[i], scene, meshes);
    }
}

void Loader::ProcessMesh(aiMesh* mesh, const aiScene* scene, std::vector<std::shared_ptr<Mesh>>& meshes)
{
    std::shared_ptr<Mesh> output_mesh = std::make_shared<Mesh>();

    // Process vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        //std::cout << mesh->mVertices[i].x << " " << mesh->mVertices[i].y << " " << mesh->mVertices[i].z << std::endl;
        output_mesh->vertices.emplace_back(glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));

        if (mesh->HasNormals()) {
            output_mesh->normals.emplace_back(glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));
            //vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        }
        else {} // do something if the model doesn't have normals

        /*if (mesh->HasTextureCoords(0)) {
            vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);*/
        //}
        //vertices.push_back(vertex);
    }

    // Process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        // calcaulte normal of the face
        auto v0_index = mesh->mFaces[i].mIndices[0];
        auto v1_index = mesh->mFaces[i].mIndices[1];
        auto v2_index = mesh->mFaces[i].mIndices[2];
        /*const glm::vec3& normal_v0 = mesh->mNormals[v0_index];*/
        const glm::vec3& normal_v0 = output_mesh->normals[v0_index];
        const glm::vec3& normal_v1 = output_mesh->normals[v1_index];
        const glm::vec3& normal_v2 = output_mesh->normals[v2_index];
        /*const glm::vec3 edge1 = normal_v1 - normal_v0;
        const glm::vec3 edge1 = normal_v2 - normal_v0;*/
        //glm::vec3 face_normal = glm::normalize(glm::cross((normal_v1 - normal_v0), (normal_v2 - normal_v0)));
        glm::vec3 face_normal = glm::normalize(normal_v0);

        aiFace face = mesh->mFaces[i];
        Face out_face(
            std::vector<unsigned int>(face.mIndices, face.mIndices + face.mNumIndices),
            face_normal
        );
        output_mesh->faces.emplace_back(out_face);
    }

    meshes.emplace_back(output_mesh);
}

std::vector<std::shared_ptr<Mesh>> Loader::Load(std::string filename)
{
    std::vector<std::shared_ptr<Mesh>> meshes;

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals);

    // return zero meshes if the file read filed
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return meshes;
    }

    aiNode* root_node = scene->mRootNode;
    Loader::ProcessNode(root_node, scene, meshes);

    return meshes;
}
