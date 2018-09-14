#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include "PrimitiveShape.h"

class ATOM_API Quad : public PrimitiveShape
{
public:
    Quad(bool fullscreenQuad);
    Quad(float width, float height, glm::vec3 color = glm::vec3(1.0,1.0,1.0));
    ~Quad();

    void Render(GLuint textureID);

    virtual void Render() override;

private:
    Buffer m_elementBuffer;

    std::vector<unsigned int> m_indices;
};

