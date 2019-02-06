#include "BoxCollider.h"
#include "Transform.h"
#include "Mesh.h"
#include "RigidBody.h" 


BoxCollider::BoxCollider()
{
    m_name = "Box Collider";
    m_typeInfo = &typeid(this);
    m_initialized = false;
    m_debugCubeAABB = new DebugCuboid(1, 1, 1, glm::vec3(0,0, 1));
    m_debugCubeOBB = new DebugCuboid(1, 1, 1, glm::vec3(1, 1, 0));
}

BoxCollider::~BoxCollider()
{
}

bool BoxCollider::Initialize()
{
    m_transform = GetComponent<Transform>();
    m_rigidBody = GetComponent<RigidBody>();

    auto mesh = GetComponent<Mesh>();
    if(mesh != nullptr)
    {
        m_minBounds = mesh->GetMinimumBounds();
        m_maxBounds = mesh->GetMaximumBounds();

        m_verticesOBB[0] = m_minBounds;
        m_verticesOBB[1] = glm::vec3(m_maxBounds.x, m_minBounds.y, m_minBounds.z);
        m_verticesOBB[2] = glm::vec3(m_minBounds.x, m_maxBounds.y, m_minBounds.z);
        m_verticesOBB[3] = glm::vec3(m_maxBounds.x, m_maxBounds.y, m_minBounds.z);
        m_verticesOBB[4] = glm::vec3(m_minBounds.x, m_minBounds.y, m_maxBounds.z);
        m_verticesOBB[5] = glm::vec3(m_maxBounds.x, m_minBounds.y, m_maxBounds.z);
        m_verticesOBB[6] = glm::vec3(m_minBounds.x, m_maxBounds.y, m_maxBounds.z);
        m_verticesOBB[7] = m_maxBounds;

        m_debugCubeOBB->SetDimensions(m_maxBounds - m_minBounds);

        m_initialized = true;
        return true;
    }
    return false;
}

void BoxCollider::Update(float deltaTime)
{
    if(m_initialized) {
        RecalculateAABB();
    }
}

EndPoint BoxCollider::GetStartPoint(const Axis axis) const
{
    auto position = m_transform->GetPosition();
    auto scale = m_transform->GetScale();

    switch(axis)
    {
    case Axis::X: return EndPoint((m_minBounds.x * scale.x) + position.x, m_rigidBody, true); break;
    case Axis::Y: return EndPoint((m_minBounds.y * scale.y) + position.y, m_rigidBody, true); break;
    case Axis::Z: return EndPoint((m_minBounds.z * scale.z) + position.z, m_rigidBody, true); break;
    default: return EndPoint(0, nullptr, false);
    }
}

EndPoint BoxCollider::GetEndPoint(const Axis axis) const
{
    auto position = m_transform->GetPosition();
    auto scale = m_transform->GetScale();

    switch (axis)
    {
    case Axis::X: return EndPoint((m_maxBounds.x * scale.x) + position.x, m_rigidBody, false); break;
    case Axis::Y: return EndPoint((m_maxBounds.y * scale.y) + position.y, m_rigidBody, false); break;
    case Axis::Z: return EndPoint((m_maxBounds.z * scale.z) + position.z, m_rigidBody, false); break;
    default: return EndPoint(0, nullptr, false);
    }
}

void BoxCollider::DrawDebug()
{
    m_debugCubeOBB->Render(m_transform->GetTransform());

    m_debugCubeAABB->SetDimensions((m_maxBounds - m_minBounds) * m_transform->GetScale());

    Transform transform;
    transform.Translate(m_transform->GetPosition() * m_transform->GetScale());
    m_debugCubeAABB->Render(transform.GetTransform());
}

void BoxCollider::RecalculateOBB(std::array<glm::vec3, 8>& vertices)
{
    const auto rotation = glm::mat3_cast(m_transform->GetRotation());
    for(auto& vert : vertices) {
        vert = rotation * vert;
    }
}

std::vector<glm::vec3> BoxCollider::RecalculateOBB()
{
    //TODO: This function takes too much time needs to be optimised!!
    const auto rotation = glm::mat3_cast(m_transform->GetRotation());
    std::vector<glm::vec3> newOBB;
    for(int i = 0; i < 8; i++) {
        newOBB.push_back(rotation * m_verticesOBB[i]);
    }
    return newOBB;
}

void BoxCollider::RecalculateAABB()
{
    auto vertices = m_verticesOBB;

    RecalculateOBB(vertices);

    for(int i = 0; i < 8; i++) {
        if(i == 0) {
            m_minBounds.x = m_maxBounds.x = vertices[i].x;
            m_minBounds.y = m_maxBounds.y = vertices[i].y;
            m_minBounds.z = m_maxBounds.z = vertices[i].z;
        }
        else {
            if (vertices[i].x > m_maxBounds.x) m_maxBounds.x = vertices[i].x;
            if (vertices[i].y > m_maxBounds.y) m_maxBounds.y = vertices[i].y;
            if (vertices[i].z > m_maxBounds.z) m_maxBounds.z = vertices[i].z;

            if (vertices[i].x < m_minBounds.x) m_minBounds.x = vertices[i].x;
            if (vertices[i].y < m_minBounds.y) m_minBounds.y = vertices[i].y;
            if (vertices[i].z < m_minBounds.z) m_minBounds.z = vertices[i].z;
        }
    }
}
