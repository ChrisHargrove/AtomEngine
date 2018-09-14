#include "Quad.h"

#include "ShaderManager.h"
#include "LogManager.h"


Quad::Quad(bool fullscreenQuad)
{
    if (fullscreenQuad) {
        m_vertices = {
            -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        };

        m_indices = {
            0,1,2,0,2,3
        };

        m_vertexArray.Create(VAO);
        m_vertexBuffer.Create(VBO);
        m_elementBuffer.Create(EBO);

        m_vertexArray.Bind();
        m_vertexBuffer.FillBuffer(sizeof(float) * m_vertices.size(), &m_vertices[0], STATIC);
        m_elementBuffer.FillBuffer(sizeof(unsigned int) * m_indices.size(), &m_indices[0], STATIC);

        m_vertexBuffer.AddAttributePointer(BufferAttribute::POSITION, 3, VT_FLOAT, 5 * sizeof(float));
        m_vertexBuffer.AddAttributePointer(BufferAttribute::UV, 2, VT_FLOAT, 5 * sizeof(float), 3 * sizeof(float));
        m_color = glm::vec3(1.0,1.0,1.0);

        m_drawCount = m_indices.size();
        m_vertexArray.Unbind();
    }
}

Quad::Quad(float width, float height, glm::vec3 color)
{
    m_vertices = {
        -0.5f * width, 0.5f * height, 0.0f,
        -0.5f * width, -0.5f * height, 0.0f,
        0.5f * width, -0.5f * height, 0.0f,
        0.5f * width, 0.5f * height, 0.0f,
    };

    m_indices = {
        0,1,2,0,2,3
    };

    m_vertexArray.Create(VAO);
    m_vertexBuffer.Create(VBO);
    m_elementBuffer.Create(EBO);

    m_vertexArray.Bind();
    m_vertexBuffer.FillBuffer(sizeof(float) * m_vertices.size(), &m_vertices[0], STATIC);
    m_elementBuffer.FillBuffer(sizeof(unsigned int) * m_indices.size(), &m_indices[0], STATIC);

    m_vertexBuffer.AddAttributePointer(BufferAttribute::POSITION, 3, VT_FLOAT, 3 * sizeof(float));
    m_color = color;

    m_drawCount = m_indices.size();
    m_vertexArray.Unbind();
    
}

Quad::~Quad()
{
    m_vertexArray.Destroy();
    m_vertexBuffer.Destroy();
}

void Quad::Render()
{
    m_vertexArray.Bind();
    glDrawElements(GL_TRIANGLES, m_drawCount, GL_UNSIGNED_INT, 0);
    m_vertexArray.Unbind();
}
