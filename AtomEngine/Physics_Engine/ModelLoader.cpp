#include "ModelLoader.h"
#include "LogManager.h"
#include "Mesh.h"
#include <vector>

#include <ASSIMP/Importer.hpp>

#include <ASSIMP/postprocess.h>

bool ModelLoader::LoadModel(const std::string & fileName, Mesh* mesh)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        Logger::Instance()->LogError("[ASSIMP]" + std::string(importer.GetErrorString()));
        return false;
    }
    
    ProcessNode(scene->mRootNode, scene, mesh);

    return true;
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

    mesh->m_subMeshList.push_back(subMesh);

}

