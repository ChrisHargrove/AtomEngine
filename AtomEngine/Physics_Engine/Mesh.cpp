#include "Mesh.h"



Mesh::Mesh() :
    m_color(glm::vec3(1, 1, 1)),
    m_drawCount(0)
{
}


Mesh::~Mesh()
{
    m_vertexArray.Destroy();
    m_vertexBuffer.Destroy();
    m_elementBuffer.Destroy();
}

void Mesh::Update(float deltaTime)
{
}

void Mesh::Initialize()
{
    if (m_meshName != "") {
        LoadMesh();
    }
    else {
        m_vertexArray.Create(VAO);
        m_vertexBuffer.Create(VBO);
        m_elementBuffer.Create(EBO);

        m_vertexArray.Bind();
        m_vertexBuffer.Bind();

        m_vertexBuffer.AddAttributePointer(BufferAttribute::POSITION, 3, VT_FLOAT, sizeof(ComplexVertex));
        m_vertexBuffer.AddAttributePointer(BufferAttribute::NORMAL, 3, VT_FLOAT, sizeof(ComplexVertex), 3 * sizeof(float));
        m_vertexBuffer.AddAttributePointer(BufferAttribute::UV, 2, VT_FLOAT, sizeof(ComplexVertex), 5 * sizeof(float));
        m_vertexBuffer.AddAttributePointer(BufferAttribute::TANGENT, 3, VT_FLOAT, sizeof(ComplexVertex), 8 * sizeof(float));
        m_vertexBuffer.AddAttributePointer(BufferAttribute::BITANGENT, 3, VT_FLOAT, sizeof(ComplexVertex), 11 * sizeof(float));

        m_elementBuffer.Bind();

        m_vertexArray.Unbind();
    }
}

void Mesh::SetMesh(const std::string& meshName)
{
    m_meshName = meshName;
}

void Mesh::LoadMesh()
{
    //TODO: Load all mesh data using ASSIMP

    //This function is to be used a loader for when reading from serialized data.
}

void Mesh::LoadMesh(const std::string& meshName)
{
    //TODO: Load all mesh data using ASSIMP

    //This function is to be used for all other mesh loading instances. But requires that the mesh be loaded and set before
    //adding to the game object.
}

void Mesh::SetColor(glm::vec3 color)
{
    m_color = color;
}
