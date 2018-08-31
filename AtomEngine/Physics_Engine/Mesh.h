#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include "Component.h"
#include "Buffer.h"
#include <GLM/glm.hpp>
#include <vector>
#include <string>

#include "Types.h"

class ATOM_API SubMesh
{
public:
    SubMesh();
    virtual ~SubMesh();

    friend class ModelLoader;
    friend class Mesh;
    void Render();

private:
    unsigned int m_drawCount;
    std::vector<ComplexVertex> m_vertices;
    std::vector<unsigned int> m_indices;

    Buffer m_vertexArray;
    Buffer m_vertexBuffer;
    Buffer m_elementBuffer;
};

class ATOM_API Mesh : public Component
{
public:
    Mesh();
    virtual ~Mesh();

    friend class ModelLoader;

    virtual void Update(float deltaTime) override;
    virtual void Initialize() override;

    void SetMesh(const std::string& meshName);

    bool LoadMesh();
    bool LoadMesh(const std::string& meshName);

    void Render();

private:
    std::string m_meshName;
    unsigned int m_drawCount;

    std::vector<SubMesh*> m_subMeshList;
};



