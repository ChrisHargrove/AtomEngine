#pragma once

#include "PrimitiveShape.h"
#include <GLM/glm.hpp>
#include <vector>

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

class ATOM_API Cuboid : public PrimitiveShape
{
public:
    Cuboid(float width, float height, float depth, glm::vec3 color = glm::vec3(1.0,1.0,1.0));
    ~Cuboid();

    virtual void Render() override;

    void SetColor(glm::vec3 color);

private:
    std::vector<float> m_vertices;
    glm::vec3 m_color;

};

