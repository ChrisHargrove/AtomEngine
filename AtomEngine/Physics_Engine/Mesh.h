#pragma once

#include "Component.h"
#include "Buffer.h"
#include <GLM/glm.hpp>
#include <vector>
#include <string>

#include "Types.h"

class Mesh : public Component
{
public:
    Mesh();
    virtual ~Mesh();

    virtual void Update(float deltaTime) override;
    virtual void Initialize() override;

    void SetMesh(const std::string& meshName);
    void LoadMesh();
    void LoadMesh(const std::string& meshName);

    void SetColor(glm::vec3 color);

    void Render();

private:
    std::string m_meshName;
    unsigned int m_drawCount;

    std::vector<ComplexVertex> m_vertices;
    std::vector<unsigned int> m_indices;

    Buffer m_vertexArray;
    Buffer m_vertexBuffer;
    Buffer m_elementBuffer;

    glm::vec3 m_color;
};

