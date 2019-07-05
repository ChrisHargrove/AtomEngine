#include "BallCollider.h"
//#include "Transform.h"
//#include "Mesh.h"
//#include "RigidBody.h" 
//
//
//BallCollider::BallCollider()
//{
//    m_name = "Ball Collider";
//    m_typeInfo = &typeid(this);
//    m_initialized = false;
//    m_debugCube = new DebugCuboid(1, 1, 1, glm::vec3(0, 0, 1));
//}
//
//BallCollider::~BallCollider()
//{
//}
//
//bool BallCollider::Initialize()
//{
//    m_transform = GetComponent<Transform>();
//    m_rigidBody = GetComponent<RigidBody>();
//    m_rigidBody->m_collider = this;
//
//    auto mesh = GetComponent<Mesh>();
//    if(mesh != nullptr)
//    {
//        m_maxBounds = mesh->GetMaximumBounds();
//        m_minBounds = mesh->GetMinimumBounds();
//
//        m_radius = (glm::length(m_maxBounds - m_minBounds)) * 0.5f;
//        m_debugCube->SetDimensions(m_maxBounds - m_minBounds);
//
//        m_initialized = true;
//
//        return true;
//    }
//    return false;
//}
//
//void BallCollider::DrawDebug()
//{
//    m_debugCube->SetDimensions((m_maxBounds - m_minBounds) * m_transform->GetScale());
//
//    Transform transform;
//    transform.Translate(m_transform->GetPosition() * m_transform->GetScale());
//    m_debugCube->Render(transform.GetTransform());
//}
