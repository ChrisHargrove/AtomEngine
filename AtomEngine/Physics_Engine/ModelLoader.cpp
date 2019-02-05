#include "ModelLoader.h"
#include "LogManager.h"
#include "Mesh.h"
#include <vector>

#include <ASSIMP/Importer.hpp>
#include <ASSIMP/postprocess.h>
#include "ResourceManager.h"

bool ModelLoader::LoadModel(const std::string & fileName, Mesh* mesh)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        Logger::Instance()->LogError("[ASSIMP]" + std::string(importer.GetErrorString()));
        return false;
    }
    
    ProcessNode(scene->mRootNode, scene, mesh);

    CalculateMeshBounds(mesh);

    return true;
}

bool ModelLoader::LoadModel(const std::string& fileName)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        Logger::Instance()->LogError("[ASSIMP]" + std::string(importer.GetErrorString()));
        return false;
    }

    Mesh mesh;
    mesh.m_meshName = fileName;
    ProcessNode(scene->mRootNode, scene, &mesh);
    CalculateMeshBounds(&mesh);

    auto resource = std::make_shared<Mesh>(mesh);
    
    return Resource::Instance()->AddResource<Mesh>(fileName, std::move(resource));
}

void ModelLoader::ProcessNode(aiNode * node, const aiScene * scene, Mesh* mesh)
{
    for (int i = 0; i < (int)node->mNumMeshes; i++) {
        aiMesh* aimesh = scene->mMeshes[node->mMeshes[i]];
        ProcessMesh(aimesh, scene, mesh);
    }
    for (int i = 0; i < (int)node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene, mesh);
    }
}

void ModelLoader::ProcessNode(aiNode* node, const aiScene* scene, MeshResource* resource)
{
    for (int i = 0; i < (int)node->mNumMeshes; i++) {
        aiMesh* aimesh = scene->mMeshes[node->mMeshes[i]];
        ProcessMesh(aimesh, scene, resource);
    }
    for (int i = 0; i < (int)node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene, resource);
    }
}

void ModelLoader::ProcessMesh(aiMesh * aimesh, const aiScene * scene, Mesh* mesh)
{
    std::vector<ComplexVertex> vertices;
    std::vector<unsigned int> indices;

    SubMesh* subMesh = new SubMesh();
    
    for (int i = 0; i < (int)aimesh->mNumVertices; i++) {
        ComplexVertex vertex;
        glm::vec3 vector;

        vector.x = aimesh->mVertices[i].x;
        vector.y = aimesh->mVertices[i].y;
        vector.z = aimesh->mVertices[i].z;
        vertex.m_position = vector;

        vector.x = aimesh->mNormals[i].x;
        vector.y = aimesh->mNormals[i].y;
        vector.z = aimesh->mNormals[i].z;
        vertex.m_normal = vector;

        if (aimesh->mTextureCoords[0]) {
            glm::vec2 vec;
            vec.x = aimesh->mTextureCoords[0][i].x;
            vec.y = aimesh->mTextureCoords[0][i].y;
            vertex.m_UV = vec;
        }
        else {
            vertex.m_UV = glm::vec2(0, 0);
        }

        if (aimesh->mTangents != NULL) {
            vector.x = aimesh->mTangents[i].x;
            vector.y = aimesh->mTangents[i].y;
            vector.z = aimesh->mTangents[i].z;
            vertex.m_tangent = vector;
        }

        if (aimesh->mBitangents != NULL) {
            vector.x = aimesh->mBitangents[i].x;
            vector.y = aimesh->mBitangents[i].y;
            vector.z = aimesh->mBitangents[i].z;
            vertex.m_bitangent = vector;
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < aimesh->mNumFaces; i++) {
        aiFace face = aimesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    //TODO: LOADING OF MODEL TEXTURES!

    subMesh->m_vertices = vertices;
    subMesh->m_indices = indices;
    subMesh->m_drawCount = indices.size();

    if (!mesh->m_subMeshList) {
        mesh->m_subMeshList = std::make_shared<SubMeshList>();
    }
    mesh->m_subMeshList->push_back(subMesh);

}

void ModelLoader::ProcessMesh(aiMesh* aimesh, const aiScene* scene, MeshResource* resource)
{
    std::vector<ComplexVertex> vertices;
    std::vector<unsigned int> indices;

    SubMesh* subMesh = new SubMesh();

    for (int i = 0; i < (int)aimesh->mNumVertices; i++) {
        ComplexVertex vertex;
        glm::vec3 vector;

        vector.x = aimesh->mVertices[i].x;
        vector.y = aimesh->mVertices[i].y;
        vector.z = aimesh->mVertices[i].z;
        vertex.m_position = vector;

        vector.x = aimesh->mNormals[i].x;
        vector.y = aimesh->mNormals[i].y;
        vector.z = aimesh->mNormals[i].z;
        vertex.m_normal = vector;

        if (aimesh->mTextureCoords[0]) {
            glm::vec2 vec;
            vec.x = aimesh->mTextureCoords[0][i].x;
            vec.y = aimesh->mTextureCoords[0][i].y;
            vertex.m_UV = vec;
        }
        else {
            vertex.m_UV = glm::vec2(0, 0);
        }

        if (aimesh->mTangents != NULL) {
            vector.x = aimesh->mTangents[i].x;
            vector.y = aimesh->mTangents[i].y;
            vector.z = aimesh->mTangents[i].z;
            vertex.m_tangent = vector;
        }

        if (aimesh->mBitangents != NULL) {
            vector.x = aimesh->mBitangents[i].x;
            vector.y = aimesh->mBitangents[i].y;
            vector.z = aimesh->mBitangents[i].z;
            vertex.m_bitangent = vector;
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < aimesh->mNumFaces; i++) {
        aiFace face = aimesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    //TODO: LOADING OF MODEL TEXTURES!

    subMesh->m_vertices = vertices;
    subMesh->m_indices = indices;
    subMesh->m_drawCount = indices.size();

    resource->m_subMeshList->push_back(subMesh);
}

void ModelLoader::CalculateMeshBounds(Mesh* mesh)
{
    glm::vec3 minBounds(0, 0, 0);
    glm::vec3 maxBounds(0, 0, 0);

    for(auto& submesh : mesh->GetSubMeshList())
    {
        for(auto vertex : submesh->m_vertices)
        {
            if(vertex.m_position.x < minBounds.x)
            {
                minBounds.x = vertex.m_position.x;
            }
            else if(vertex.m_position.x > maxBounds.x)
            {
                maxBounds.x = vertex.m_position.x;
            }

            if (vertex.m_position.y < minBounds.y)
            {
                minBounds.y = vertex.m_position.y;
            }
            else if (vertex.m_position.y > maxBounds.y)
            {
                maxBounds.y = vertex.m_position.y;
            }

            if (vertex.m_position.z < minBounds.z)
            {
                minBounds.z = vertex.m_position.z;
            }
            else if (vertex.m_position.z > maxBounds.z)
            {
                maxBounds.z = vertex.m_position.z;
            }
        }
    }

    mesh->GetMaximumBounds() = maxBounds;
    mesh->GetMinimumBounds() = minBounds;
}

void ModelLoader::CalculateMeshBounds(MeshResource* resource)
{
    glm::vec3 minBounds(0, 0, 0);
    glm::vec3 maxBounds(0, 0, 0);

    for (auto& submesh : *resource->m_subMeshList)
    {
        for (auto vertex : submesh->m_vertices)
        {
            if (vertex.m_position.x < minBounds.x)
            {
                minBounds.x = vertex.m_position.x;
            }
            else if (vertex.m_position.x > maxBounds.x)
            {
                maxBounds.x = vertex.m_position.x;
            }

            if (vertex.m_position.y < minBounds.y)
            {
                minBounds.y = vertex.m_position.y;
            }
            else if (vertex.m_position.y > maxBounds.y)
            {
                maxBounds.y = vertex.m_position.y;
            }

            if (vertex.m_position.z < minBounds.z)
            {
                minBounds.z = vertex.m_position.z;
            }
            else if (vertex.m_position.z > maxBounds.z)
            {
                maxBounds.z = vertex.m_position.z;
            }
        }
    }

    resource->m_maxBounds = maxBounds;
    resource->m_minBounds = minBounds;
}

