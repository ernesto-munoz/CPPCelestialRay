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

    const aiMatrix4x4& transform = scene->mRootNode->FindNode(mesh->mName)->mTransformation;
    // Process vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        //std::cout << mesh->mVertices[i].x << " " << mesh->mVertices[i].y << " " << mesh->mVertices[i].z << std::endl;
        aiVector3D vertex = transform * mesh->mVertices[i];
        output_mesh->vertices.emplace_back(glm::vec3(vertex.x, vertex.y, vertex.z));

        if (mesh->HasNormals()) {
            aiVector3D normal = transform * mesh->mNormals[i];
            output_mesh->normals.emplace_back(glm::vec3(normal.x, normal.y, normal.z));
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
        glm::vec3 face_normal = glm::normalize(normal_v2 + normal_v1 + normal_v0);

        aiFace face = mesh->mFaces[i];
        Face out_face(
            std::vector<unsigned int>(face.mIndices, face.mIndices + face.mNumIndices),
            face_normal
        );
        output_mesh->faces.emplace_back(out_face);
    }

    output_mesh->CalculateBBox();
    meshes.emplace_back(output_mesh);

    std::cout << "Mesh: " << output_mesh->vertices.size() << " vertices, " << output_mesh->faces.size() << " faces." << std::endl;
}

const aiScene* Loader::OpenFile(const std::string& filename)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

    // return zero meshes if the file read filed
  /*  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        throw std::runtime_error(std::format("ASSIMP::ERROR::ReadFile returned an invalid scene ({})", filename));
    }*/
    return scene;
}

void Loader::GetCameras(const std::string& filename, std::vector<Camera>& cameras)
{
    /*const aiScene* scene = OpenFile(filename);*/
    // TODO: move this to a unique function
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

    for (unsigned int i = 0; i < scene->mNumCameras; ++i) {
        const aiCamera* camera = scene->mCameras[i];
        Camera final_camera = FromAssimpCamera(*camera, *scene);
        cameras.emplace_back(final_camera);
    }

}

Camera Loader::FromAssimpCamera(const aiCamera& camera, const aiScene& scene)
{
    // the node with the same name that the camera must exists, should i check it?
    const aiMatrix4x4& transform = scene.mRootNode->FindNode(camera.mName)->mTransformation;
    aiVector3D position = transform * camera.mPosition;
    aiVector3D lookat = transform * camera.mLookAt;
    aiVector3D up = camera.mUp;  // TODO: this up is not being transformed
    float vfov = glm::degrees(camera.mHorizontalFOV / camera.mAspect);

    Camera final_camera;
    final_camera.name = std::string(camera.mName.C_Str());
    final_camera.center = glm::vec3(position.x, position.y, position.z);
    final_camera.lookat = glm::vec3(lookat.x, lookat.y, lookat.z);
    final_camera.vup = glm::normalize(glm::vec3(up.x, up.y, up.z));
    final_camera.vfov = vfov;

    return final_camera;
}

void Loader::GetMeshes(const std::string& filename, std::vector<std::shared_ptr<Mesh>>& meshes)
{

    // TODO: move this to a unique function
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

    aiNode* root_node = scene->mRootNode;
    Loader::ProcessNode(root_node, scene, meshes);
}
