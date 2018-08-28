#include "Cuboid.h"

#include "ShaderManager.h"
#include "LogManager.h"

Cuboid::Cuboid(float width, float height, float depth, glm::vec3 color)
{
    m_vertices = {
        // positions          // normals           // texture coords
        -0.5f * width, -0.5f * height, -0.5f * depth,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        0.5f * width, -0.5f * height, -0.5f * depth,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
        0.5f * width,  0.5f * height, -0.5f * depth,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        0.5f * width,  0.5f * height, -0.5f * depth,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f * width,  0.5f * height, -0.5f * depth,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f * width, -0.5f * height, -0.5f * depth,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f * width, -0.5f * height,  0.5f * depth,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
        0.5f * width, -0.5f * height,  0.5f * depth,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
        0.5f * width,  0.5f * height,  0.5f * depth,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        0.5f * width,  0.5f * height,  0.5f * depth,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f * width,  0.5f * height,  0.5f * depth,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f * width, -0.5f * height,  0.5f * depth,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f * width,  0.5f * height,  0.5f * depth, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f * width,  0.5f * height, -0.5f * depth, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f * width, -0.5f * height, -0.5f * depth, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f * width, -0.5f * height, -0.5f * depth, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f * width, -0.5f * height,  0.5f * depth, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f * width,  0.5f * height,  0.5f * depth, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        0.5f * width,  0.5f * height,  0.5f * depth,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        0.5f * width,  0.5f * height, -0.5f * depth,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        0.5f * width, -0.5f * height, -0.5f * depth,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        0.5f * width, -0.5f * height, -0.5f * depth,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        0.5f * width, -0.5f * height,  0.5f * depth,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        0.5f * width,  0.5f * height,  0.5f * depth,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f * width, -0.5f * height, -0.5f * depth,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
        0.5f * width, -0.5f * height, -0.5f * depth,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
        0.5f * width, -0.5f * height,  0.5f * depth,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        0.5f * width, -0.5f * height,  0.5f * depth,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f * width, -0.5f * height,  0.5f * depth,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f * width, -0.5f * height, -0.5f * depth,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f * width,  0.5f * height, -0.5f * depth,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
        0.5f * width,  0.5f * height, -0.5f * depth,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
        0.5f * width,  0.5f * height,  0.5f * depth,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        0.5f * width,  0.5f * height,  0.5f * depth,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f * width,  0.5f * height,  0.5f * depth,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f * width,  0.5f * height, -0.5f * depth,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
    };

    m_vertexArray.Create(VAO);
    m_vertexBuffer.Create(VBO);

    m_vertexArray.Bind();
    m_vertexBuffer.FillBuffer(sizeof(float) * m_vertices.size(), &m_vertices[0], STATIC);

    m_vertexBuffer.AddAttributePointer(0, 3, VT_FLOAT, 8 * sizeof(float), 0); 
    m_color = color;

    m_drawCount = 36;
    m_vertexArray.Unbind();
}


Cuboid::~Cuboid()
{
    m_vertexArray.Destroy();
    m_vertexBuffer.Destroy();
}

void Cuboid::Render()
{
    Shaders::Instance()->GetShader("BASIC")->SetVec3("aColor", m_color);
    m_vertexArray.Bind();
    glDrawArrays(GL_TRIANGLES, 0, m_drawCount);
    m_vertexArray.Unbind();
}

void Cuboid::SetColor(glm::vec3 color)
{
    m_color = color;
}
