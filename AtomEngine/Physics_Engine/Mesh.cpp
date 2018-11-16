#include "Mesh.h"
#include "ModelLoader.h"
#include "LogManager.h"

#include "ShaderManager.h"


Mesh::Mesh()
{
    m_name = "Mesh";
    m_typeInfo = &typeid(this);
}


Mesh::~Mesh()
{
    for (int i = 0; i < m_subMeshList.size(); i++) {
        delete m_subMeshList[i];
    }
}

void Mesh::Update(float deltaTime)
{
}

void Mesh::Initialize()
{
    if (m_meshName == "") {
        return;
    }
    else {
        if (LoadMesh(m_meshName)) {

            for (auto subMesh : m_subMeshList) {
                subMesh->m_vertexArray.Bind();
                subMesh->m_vertexBuffer.Bind();

                subMesh->m_vertexBuffer.FillBuffer(sizeof(ComplexVertex) * subMesh->m_vertices.size(), &subMesh->m_vertices[0], STATIC);

                subMesh->m_vertexBuffer.AddAttributePointer(BufferAttribute::POSITION, 3, VT_FLOAT, sizeof(ComplexVertex));
                subMesh->m_vertexBuffer.AddAttributePointer(BufferAttribute::NORMAL, 3, VT_FLOAT, sizeof(ComplexVertex), 3 * sizeof(float));
                subMesh->m_vertexBuffer.AddAttributePointer(BufferAttribute::UV, 2, VT_FLOAT, sizeof(ComplexVertex), 5 * sizeof(float));
                subMesh->m_vertexBuffer.AddAttributePointer(BufferAttribute::TANGENT, 3, VT_FLOAT, sizeof(ComplexVertex), 8 * sizeof(float));
                subMesh->m_vertexBuffer.AddAttributePointer(BufferAttribute::BITANGENT, 3, VT_FLOAT, sizeof(ComplexVertex), 11 * sizeof(float));

                subMesh->m_elementBuffer.Bind();
                subMesh->m_elementBuffer.FillBuffer(sizeof(unsigned int) * subMesh->m_indices.size(), &subMesh->m_indices[0], STATIC);

                subMesh->m_vertexArray.Unbind();
            }
            
        }
    }
}

void Mesh::SetMesh(const std::string& meshName)
{
    m_meshName = meshName;
}

const std::string& Mesh::GetMesh()
{
    return m_meshName;
}

bool Mesh::LoadMesh()
{
    return LoadMesh(m_meshName);
}

bool Mesh::LoadMesh(const std::string& meshName)
{
    if (ModelLoader::LoadModel(meshName, this)) {
        Logger::Instance()->LogInfo("Successfully Loaded: " + meshName);
        return true;
    }
    return false;
}

void Mesh::Render()
{
    for (auto subMesh : m_subMeshList) {
        if (subMesh->m_drawCount > 0) {
            subMesh->Render();
        }
    }
}

void Mesh::Render(int instanceCount)
{
    for (auto subMesh : m_subMeshList) {
        if (subMesh->m_drawCount > 0) {
            subMesh->Render(instanceCount);
        }
    }
}

std::vector<SubMesh*> Mesh::GetSubMeshList()
{
    return m_subMeshList;
}

SubMesh::SubMesh() :
    m_drawCount(0)
{
    m_vertexArray.Create(VAO);
    m_vertexBuffer.Create(VBO);
    m_elementBuffer.Create(EBO);
}

SubMesh::~SubMesh()
{

    m_vertexArray.Destroy();
    m_vertexBuffer.Destroy();
    m_elementBuffer.Destroy();
}

void SubMesh::Render()
{
    m_vertexArray.Bind();
    glDrawElements(GL_TRIANGLES, m_drawCount, GL_UNSIGNED_INT, 0);
    m_vertexArray.Unbind();
}

void SubMesh::Render(int instanceCount)
{
    m_vertexArray.Bind();
    glDrawElementsInstanced(GL_TRIANGLES, m_drawCount, GL_UNSIGNED_INT, 0, instanceCount);
    m_vertexArray.Unbind();
}

Buffer* SubMesh::GetVAO()
{
    return &m_vertexArray;
}
