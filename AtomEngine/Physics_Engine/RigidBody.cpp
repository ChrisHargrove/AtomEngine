#include "RigidBody.h"
#include "Transform.h"
#include "LogManager.h"

RigidBody::RigidBody()
{
    m_name = "Rigid Body";
    m_typeInfo = &typeid(this);
    m_affectedByGravity = false;
}

RigidBody::~RigidBody()
{
}

bool RigidBody::Initialize()
{
    m_linearMomentum = glm::vec3(0,0,0);
    m_angularMomentum = glm::vec3(0, 0, 0);

    m_mass = 1.0f;

    CalculateInertiaTensorIntegral(GetComponent<Transform>()->GetScale());

    return true;
}

void RigidBody::Simulate(float deltaTime)
{

    auto velocity = (m_linearMomentum / m_mass) * deltaTime;

    GetComponent<Transform>()->Translate(velocity);

    auto orientationMatrix = glm::mat3_cast(GetComponent<Transform>()->GetRotation());

    auto inverseInertia = orientationMatrix * m_inverseInertiaTensorIntegral * glm::transpose(orientationMatrix);

    auto angularVelocity = (inverseInertia * m_angularMomentum) * deltaTime;

    auto q = glm::quat(0, angularVelocity.x, angularVelocity.y, angularVelocity.z);

    auto spin = 0.5f * (q * GetComponent<Transform>()->GetRotation());

    GetComponent<Transform>()->GetRotation() += spin;
}

void RigidBody::ApplyForce(glm::vec3 force)
{
    m_linearMomentum += force;
}

void RigidBody::ApplyForce(float x, float y, float z)
{
    m_linearMomentum += glm::vec3(x, y, z);
}

void RigidBody::ApplyForce(glm::vec3 force, glm::vec3 position)
{
    m_linearMomentum += force;
    m_angularMomentum += glm::cross(position - GetComponent<Transform>()->GetPosition(), force);
}

void RigidBody::ApplyTorque(glm::vec3 torque)
{
    m_angularMomentum += torque;
}

float& RigidBody::GetMass()
{
    return m_mass;
}

void RigidBody::SetMass(float mass)
{
    m_mass = mass;
}

bool& RigidBody::UsesGravity()
{
    return m_affectedByGravity;
}

void RigidBody::CalculateInertiaTensorIntegral()
{
    auto xSqr = m_dimensions.x * m_dimensions.x;
    auto ySqr = m_dimensions.y * m_dimensions.y;
    auto zSqr = m_dimensions.z * m_dimensions.z;

    auto inertiaMat = glm::mat3(
        glm::vec3(ySqr + zSqr, 0, 0),
        glm::vec3(0, xSqr + zSqr, 0),
        glm::vec3(0, 0, xSqr + ySqr)
    );

    m_inertiaTensorIntegral = (m_mass / 12.0f) * inertiaMat;
    m_inverseInertiaTensorIntegral = glm::inverse(m_inertiaTensorIntegral);
}

void RigidBody::CalculateInertiaTensorIntegral(glm::vec3 dimensions)
{
    m_dimensions = dimensions;

    auto xSqr = dimensions.x * dimensions.x;
    auto ySqr = dimensions.y * dimensions.y;
    auto zSqr = dimensions.z * dimensions.z;

    auto inertiaMat = glm::mat3(
        glm::vec3(ySqr + zSqr, 0, 0),
        glm::vec3(0, xSqr + zSqr, 0),
        glm::vec3(0, 0, xSqr + ySqr)
    );

    m_inertiaTensorIntegral = (m_mass / 12.0f) * inertiaMat;
    m_inverseInertiaTensorIntegral = glm::inverse(m_inertiaTensorIntegral);
}


