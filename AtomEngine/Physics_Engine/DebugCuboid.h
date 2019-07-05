#pragma once

#include "PrimitiveShape.h"
#include <GLM/glm.hpp>
#include <vector>

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

class ATOM_API DebugCuboid : public PrimitiveShape
{
public:
    DebugCuboid(float width = 1.0f, float height = 1.0f, float depth = 1.0f, glm::vec3 color = glm::vec3(1.0,1.0,1.0));
    ~DebugCuboid();

    void SetDimensions(float width, float height, float depth);
    void SetDimensions(glm::vec3 dimensions);

    Buffer* GetVertexArray() {
        return &m_vertexArray;
    }

    void FillInstanceBuffer(std::vector<glm::mat4>& data) {
        m_instanceBuffer.FillBuffer(data.size() * sizeof(glm::mat4), &data[0], DYNAMIC);
        m_instanceBuffer.Unbind();
    }

    unsigned int GetDrawCount() {
        return m_drawCount;
    }

    void Render() override {};

    glm::vec3 GetColor()
    {
        return m_color;
    }

private:
    Buffer m_elementBuffer;
    Buffer m_instanceBuffer;

    std::vector<unsigned int> m_indices;

};

