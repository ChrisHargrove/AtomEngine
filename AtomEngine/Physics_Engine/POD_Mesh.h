#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include <vector>
#include "Types.h"
#include "Buffer.h"

class POD_SubMesh
{
public:
    friend class ModelLoader;
    friend class POD_Mesh;
    friend class Renderer;

    POD_SubMesh() :
        m_drawCount(0) {
        m_vertexArray.Create(VAO);
        m_vertexBuffer.Create(VBO);
        m_elementBuffer.Create(EBO);
        m_instanceBuffer.Create(VBO);
    }

    ~POD_SubMesh() {
        m_vertexArray.Destroy();
        m_vertexBuffer.Destroy();
        m_elementBuffer.Destroy();
        m_instanceBuffer.Destroy();
    }

    void Bind() {
        m_vertexArray.Bind();
    }
    void Unbind() {
        m_vertexArray.Unbind();
    }
    void FillInstanceBuffer(std::vector<glm::mat4>& data) {
        m_instanceBuffer.FillBuffer(data.size() * sizeof(glm::mat4), &data[0], DYNAMIC);
        m_instanceBuffer.Unbind();
    }

protected:
    unsigned int m_drawCount;
    std::vector<ComplexVertex> m_vertices;
    std::vector<unsigned int> m_indices;

    Buffer m_vertexArray;
    Buffer m_vertexBuffer;
    Buffer m_elementBuffer;
    Buffer m_instanceBuffer;
};

class ATOM_API POD_Mesh
{
public:
    friend class ModelLoader;

    POD_Mesh() {};
    ~POD_Mesh(){};

    bool LoadMesh(const std::string& meshName);

    inline const std::string& GetMeshName() const {
        return m_meshName;
    }

    inline void GetBounds(glm::vec3& min, glm::vec3& max) {
        min = m_minimumBounds;
        max = m_maximumBounds;
    }

    inline std::vector<POD_SubMesh*>& GetSubmeshList() {
        return m_subMeshList;
    }

protected:
    glm::vec3 m_minimumBounds;
    glm::vec3 m_maximumBounds;

    std::string m_meshName;
    std::vector<POD_SubMesh*> m_subMeshList;
};

