#include "DebugCuboid.h"
#include "ShaderManager.h"


DebugCuboid::DebugCuboid(float width, float height, float depth, glm::vec3 color)
{
    m_color = color;

    m_vertices = {
        //front bottom left
        -0.5f * width, -0.5f * height, -0.5f * depth,
        //front bottom right
        0.5f * width, -0.5f * height, -0.5f * depth,
        //front top right
        0.5f * width, 0.5f * height, -0.5f * depth,
        //front top left
        -0.5f * width, 0.5f * height, -0.5f * depth,

        //back bottom left
        -0.5f * width, -0.5f * height, 0.5f * depth,
        //back bottom right
        0.5f * width, -0.5f * height, 0.5f * depth,
        //back top right
        0.5f * width, 0.5f * height, 0.5f * depth,
        //back top left
        -0.5f * width, 0.5f * height, 0.5f * depth,
        
    };

    m_indices = {
        0,1,2,3,0,4,5,6,7,4,7,3,7,6,2,1,5
    };

    m_vertexArray.Create(VAO);
    m_vertexBuffer.Create(VBO);
    m_elementBuffer.Create(EBO);
    m_instanceBuffer.Create(VBO);

    m_vertexArray.Bind();
    m_vertexBuffer.Bind();

    m_vertexBuffer.FillBuffer(sizeof(float) * m_vertices.size(), &m_vertices[0], DYNAMIC);
    m_vertexBuffer.AddAttributePointer(BufferAttribute::POSITION, 3, VT_FLOAT, 3 * sizeof(float));

    m_elementBuffer.Bind();
    m_elementBuffer.FillBuffer(sizeof(unsigned int) * m_indices.size(), &m_indices[0], STATIC);

    m_instanceBuffer.Bind();
    m_instanceBuffer.AddAttributePointer(BufferAttribute::INSTANCE_MAT_1, 4, VT_FLOAT, 4 * sizeof(glm::vec4));
    m_instanceBuffer.AddAttributePointer(BufferAttribute::INSTANCE_MAT_2, 4, VT_FLOAT, 4 * sizeof(glm::vec4), sizeof(glm::vec4));
    m_instanceBuffer.AddAttributePointer(BufferAttribute::INSTANCE_MAT_3, 4, VT_FLOAT, 4 * sizeof(glm::vec4), 2 * sizeof(glm::vec4));
    m_instanceBuffer.AddAttributePointer(BufferAttribute::INSTANCE_MAT_4, 4, VT_FLOAT, 4 * sizeof(glm::vec4), 3 * sizeof(glm::vec4));
    m_instanceBuffer.AddAtributeDivisor(BufferAttribute::INSTANCE_MAT_1, 1);
    m_instanceBuffer.AddAtributeDivisor(BufferAttribute::INSTANCE_MAT_2, 1);
    m_instanceBuffer.AddAtributeDivisor(BufferAttribute::INSTANCE_MAT_3, 1);
    m_instanceBuffer.AddAtributeDivisor(BufferAttribute::INSTANCE_MAT_4, 1);

    m_vertexArray.Unbind();

    m_color = color;
    m_drawCount = m_indices.size();
}


DebugCuboid::~DebugCuboid()
{
    m_vertexArray.Destroy();
    m_vertexBuffer.Destroy();
}

void DebugCuboid::SetDimensions(float width, float height, float depth)
{
    SetDimensions(glm::vec3(width, height, depth));
}

void DebugCuboid::SetDimensions(glm::vec3 dimensions)
{
    m_vertices = {
        //front bottom left
        -0.5f * dimensions.x, -0.5f * dimensions.y, -0.5f * dimensions.z,
        //front bottom right
        0.5f * dimensions.x, -0.5f * dimensions.y, -0.5f * dimensions.z,
        //front top right
        0.5f * dimensions.x, 0.5f * dimensions.y, -0.5f * dimensions.z,
        //front top left
        -0.5f * dimensions.x, 0.5f * dimensions.y, -0.5f * dimensions.z,

        //back bottom left
        -0.5f * dimensions.x, -0.5f * dimensions.y, 0.5f * dimensions.z,
        //back bottom right
        0.5f * dimensions.x, -0.5f * dimensions.y, 0.5f * dimensions.z,
        //back top right
        0.5f * dimensions.x, 0.5f * dimensions.y, 0.5f * dimensions.z,
        //back top left
        -0.5f * dimensions.x, 0.5f * dimensions.y, 0.5f * dimensions.z,

    };

    m_vertexArray.Bind();
    m_vertexBuffer.FillBuffer(sizeof(float) * m_vertices.size(), &m_vertices[0], DYNAMIC);
    m_vertexArray.Unbind();
}
