#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include <ASSIMP/scene.h>
#include <string>

class Mesh;

class ATOM_API ModelLoader
{
public:
    static bool LoadModel(const std::string& fileName, Mesh* mesh);

private:
    static void ProcessNode(aiNode* node, const aiScene* scene, Mesh* mesh);
    static void ProcessMesh(aiMesh* aimesh, const aiScene* scene, Mesh* mesh);

};

