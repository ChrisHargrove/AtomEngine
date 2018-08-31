#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include "Buffer.h"
#include <GLM/glm.hpp>
#include <vector>

class ATOM_API PrimitiveShape
{
public:
    PrimitiveShape() {}
    virtual ~PrimitiveShape() {}

    virtual void Render() = 0;

    virtual void SetColor(glm::vec3 color) { m_color = color; }

protected:
    unsigned int m_drawCount;

    std::vector<float> m_vertices;

    Buffer m_vertexArray;
    Buffer m_vertexBuffer;

    glm::vec3 m_color;
};

