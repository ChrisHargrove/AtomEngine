#include "DebugCuboid.h"
#include "ShaderManager.h"



DebugCuboid::DebugCuboid(float width, float height, float depth, glm::vec3 color)
{

    m_vertices = {
        //front bottom left
        -0.5f * width, -0.5f * height, -0.5f * depth,
        //front botttom right
        0.5f * width, -0.5f * height, -0.5f * depth,
        //front top right
        0.5f * width, 0.5f * height, -0.5f * depth,
        //front top left
        -0.5f * width, 0.5f * height, -0.5f * depth,

        //back bottom left
        -0.5f * width, -0.5f * height, 0.5f * depth,
        //back botttom right
        0.5f * width, -0.5f * height, 0.5f * depth,
        //back top right
        0.5f * width, 0.5f * height, 0.5f * depth,
        //back top left
        -0.5f * width, 0.5f * height, 0.5f * depth,
        
    };

    m_indices = {
        0,1,2,3,0,4,5,6,7,4,7, 3,7,6,2,1,5
    };

    m_vertexArray.Create(VAO);
    m_vertexBuffer.Create(VBO);

    m_elementBuffer.Create(EBO);

    m_vertexArray.Bind();
    m_vertexBuffer.FillBuffer(sizeof(float) * m_vertices.size(), &m_vertices[0], STATIC);

    m_elementBuffer.FillBuffer(sizeof(unsigned int) * m_indices.size(), &m_indices[0], STATIC);

    m_vertexBuffer.AddAttributePointer(0, 3, VT_FLOAT, 3 * sizeof(float), 0);
    m_color = color;
    m_drawCount = m_indices.size();
    m_vertexArray.Unbind();

}


DebugCuboid::~DebugCuboid()
{
    m_vertexArray.Destroy();
    m_vertexBuffer.Destroy();
}

void DebugCuboid::Render()
{
    Shaders::Instance()->GetShader("BASIC")->SetVec3("aColor", m_color);
    m_vertexArray.Bind();
    //glDrawArrays(GL_LINES, 0, m_drawCount);
    glDrawElements(GL_LINE_STRIP, m_drawCount, GL_UNSIGNED_INT, 0);
    m_vertexArray.Unbind();
}
