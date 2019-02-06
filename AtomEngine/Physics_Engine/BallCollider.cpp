#include "BallCollider.h"
#include "Transform.h"
#include "Mesh.h"
#include "RigidBody.h" 


BallCollider::BallCollider()
{
    m_name = "Ball Collider";
    m_typeInfo = &typeid(this);
    m_initialized = false;
    m_debugCube = new DebugCuboid(1, 1, 1, glm::vec3(0, 0, 1));
}

BallCollider::~BallCollider()
{
}

bool BallCollider::Initialize()
{
    m_transform = GetComponent<Transform>();
    m_rigidBody = GetComponent<RigidBody>();

    auto mesh = GetComponent<Mesh>();
    if(mesh != nullptr)
    {
        m_maxBounds = mesh->GetMaximumBounds();
        m_minBounds = mesh->GetMinimumBounds();

        m_radius = (glm::length(m_maxBounds - m_minBounds)) * 0.5f;
        m_debugCube->SetDimensions(m_maxBounds - m_minBounds);

        m_initialized = true;

        return true;
    }
    return false;
}

EndPoint BallCollider::GetStartPoint(const Axis axis) const
{
    auto position = m_transform->GetPosition();
    auto scale = m_transform->GetScale();

    switch (axis)
    {
    case Axis::X: return EndPoint((m_minBounds.x * scale.x) + position.x, m_rigidBody, true); break;
    case Axis::Y: return EndPoint((m_minBounds.y * scale.y) + position.y, m_rigidBody, true); break;
    case Axis::Z: return EndPoint((m_minBounds.z * scale.z) + position.z, m_rigidBody, true); break;
    default: return EndPoint(0, nullptr, false);
    }
}

EndPoint BallCollider::GetEndPoint(const Axis axis) const
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

void BallCollider::DrawDebug()
{
    m_debugCube->SetDimensions((m_maxBounds - m_minBounds) * m_transform->GetScale());

    Transform transform;
    transform.Translate(m_transform->GetPosition() * m_transform->GetScale());
    m_debugCube->Render(transform.GetTransform());
}
