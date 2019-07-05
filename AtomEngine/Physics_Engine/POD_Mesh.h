#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include <vector>
#include "Types.h"
#include "Buffer.h"
#include "POD_Transform.h"

class POD_SubMesh
{
public:
    friend class ModelLoader;
    friend class POD_Mesh;

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

    unsigned int GetDrawCount() {
        return m_drawCount;
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

    inline glm::vec3 Support(POD_Transform& transform, glm::vec3 direction)
    {
        std::vector<glm::vec3> verts;
        for(auto mesh : m_subMeshList) {
            for(auto vertex : mesh->m_vertices) {
                auto vert = glm::mat3(transform.GetMatrix()) * vertex.m_position;
                vert += transform.GetPosition();
                verts.push_back(vert);
            }
        }
        float furthestDistance = -std::numeric_limits<float>::infinity();
        glm::vec3 furthestPoint(furthestDistance);
        for(auto vert : verts) {
            auto tmp = glm::dot(vert, direction);
            if(tmp > furthestDistance) {
                furthestDistance = tmp;
                furthestPoint = vert;
            }
        }

        return furthestPoint;
    }

protected:
    glm::vec3 m_minimumBounds;
    glm::vec3 m_maximumBounds;

    std::string m_meshName;
    std::vector<POD_SubMesh*> m_subMeshList;
};

