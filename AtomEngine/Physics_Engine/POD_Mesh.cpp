#include "POD_Mesh.h"
#include "ResourceManager.h"
#include "ModelLoader.h"

bool POD_Mesh::LoadMesh(const std::string& meshName)
{
    if(Resource::Instance()->HasResource<POD_Mesh>(meshName)) {
        auto resource = Resource::Instance()->GetResource<POD_Mesh>(meshName);
        m_subMeshList = resource->m_subMeshList;
        m_meshName = resource->m_meshName;
        m_minimumBounds = resource->m_minimumBounds;
        m_maximumBounds = resource->m_maximumBounds;
    }
    else if (ModelLoader::LoadModel(meshName)) {
        Logger::Instance()->LogInfo("Successfully Loaded: " + meshName);
        //m_subMeshList = new std::vector<POD_SubMesh>();
        auto resource = Resource::Instance()->GetResource<POD_Mesh>(meshName);
        m_subMeshList = resource->m_subMeshList;
        m_meshName = resource->m_meshName;
        m_minimumBounds = resource->m_minimumBounds;
        m_maximumBounds = resource->m_maximumBounds;
    }
    else {
        return false;
    }

    for(auto sub : m_subMeshList) {
        //Bind Vertex Array
        sub->m_vertexArray.Bind();
        //Bind Vertex Buffer
        sub->m_vertexBuffer.Bind();
        //Fill Vertex Buffer with data.
        sub->m_vertexBuffer.FillBuffer(sizeof(ComplexVertex) * sub->m_vertices.size(), &sub->m_vertices[0], STATIC);
        //Setup All Attribute Pointers for Vertex Buffer.
        sub->m_vertexBuffer.AddAttributePointer(BufferAttribute::POSITION, 3, VT_FLOAT, sizeof(ComplexVertex));
        sub->m_vertexBuffer.AddAttributePointer(BufferAttribute::NORMAL, 3, VT_FLOAT, sizeof(ComplexVertex), 3 * sizeof(float));
        sub->m_vertexBuffer.AddAttributePointer(BufferAttribute::UV, 2, VT_FLOAT, sizeof(ComplexVertex), 5 * sizeof(float));
        sub->m_vertexBuffer.AddAttributePointer(BufferAttribute::TANGENT, 3, VT_FLOAT, sizeof(ComplexVertex), 8 * sizeof(float));
        sub->m_vertexBuffer.AddAttributePointer(BufferAttribute::BITANGENT, 3, VT_FLOAT, sizeof(ComplexVertex), 11 * sizeof(float));
        //Bind Element Buffer.
        sub->m_elementBuffer.Bind();
        //Fill element buffer with index data.
        sub->m_elementBuffer.FillBuffer(sizeof(unsigned int) * sub->m_indices.size(), &sub->m_indices[0], STATIC);
        //Bind Instance Buffer.
        sub->m_instanceBuffer.Bind();
        //Setup instance buffer attribute pointers.
        sub->m_instanceBuffer.AddAttributePointer(BufferAttribute::INSTANCE_MAT_1, 4, VT_FLOAT, 4 * sizeof(glm::vec4));
        sub->m_instanceBuffer.AddAttributePointer(BufferAttribute::INSTANCE_MAT_2, 4, VT_FLOAT, 4 * sizeof(glm::vec4), sizeof(glm::vec4));
        sub->m_instanceBuffer.AddAttributePointer(BufferAttribute::INSTANCE_MAT_3, 4, VT_FLOAT, 4 * sizeof(glm::vec4), 2 * sizeof(glm::vec4));
        sub->m_instanceBuffer.AddAttributePointer(BufferAttribute::INSTANCE_MAT_4, 4, VT_FLOAT, 4 * sizeof(glm::vec4), 3 * sizeof(glm::vec4));
        //Setup instance buffer attribute divisors.
        sub->m_instanceBuffer.AddAtributeDivisor(BufferAttribute::INSTANCE_MAT_1, 1);
        sub->m_instanceBuffer.AddAtributeDivisor(BufferAttribute::INSTANCE_MAT_2, 1);
        sub->m_instanceBuffer.AddAtributeDivisor(BufferAttribute::INSTANCE_MAT_3, 1);
        sub->m_instanceBuffer.AddAtributeDivisor(BufferAttribute::INSTANCE_MAT_4, 1);
        //Unbind Vertex Array. Setup is done.
        sub->m_vertexArray.Unbind();
    }
    return true;
}
