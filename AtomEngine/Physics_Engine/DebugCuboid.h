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
    DebugCuboid(float width, float height, float depth, glm::vec3 color = glm::vec3(1.0,1.0,1.0));
    ~DebugCuboid();

    virtual void Render() override;

private:
    Buffer m_elementBuffer;

    std::vector<unsigned int> m_indices;

};

