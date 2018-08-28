#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include "Buffer.h"
#include <GLM/glm.hpp>

class ATOM_API PrimitiveShape
{
public:
    PrimitiveShape() {}
    virtual ~PrimitiveShape() {}

    virtual void Render() = 0;

protected:
    unsigned int m_drawCount;

    Buffer m_vertexArray;
    Buffer m_vertexBuffer;
};

