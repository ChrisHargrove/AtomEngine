#pragma once
#include <GLM/glm.hpp>
#include "POD_Transform.h"

class POD_RigidBody
{
public:
    friend class PhysicsMovementSystem;

    POD_RigidBody(POD_Transform& transformIn) :
        m_mass(1.0f),
        m_gravity(false),
        m_linearMomentum(glm::vec3(0,0,0)),
        m_angularMomentum(glm::vec3(0,0,0)),
        m_transform(transformIn)
    {
        CalculateInertiaTensorIntegral();
    }

    ~POD_RigidBody(){}

    inline void ApplyForce(glm::vec3 force) {
        m_linearMomentum += force;
    }

    inline void ApplyForce(float x, float y, float z) {
        m_linearMomentum += glm::vec3(x, y, z);
    }

    inline void ApplyForceAtPosition(glm::vec3 force, glm::vec3 position) {
        m_linearMomentum += force;
        m_angularMomentum += glm::cross(position - m_transform.GetPosition(), force);
    }

    inline void ApplyTorque(glm::vec3 torque) {
        m_angularMomentum += torque;
    }

    inline void ApplyTorque(float x, float y, float z) {
        m_angularMomentum += glm::vec3(x, y, z);
    }

    inline float GetMass() {
        return m_mass;
    }

    inline void SetMass(float mass) {
        m_mass = mass;
        CalculateInertiaTensorIntegral();
    }

    inline bool UsesGravity() {
        return m_gravity;
    }

    inline void UseGravity(bool enable = true) {
        m_gravity = true;
    }

protected:

    inline void CalculateInertiaTensorIntegral() {
        glm::vec3 scale = m_transform.GetScale();

        auto xSqr = scale.x * scale.x;
        auto ySqr = scale.y * scale.y;
        auto zSqr = scale.z * scale.z;

        auto inertiaMat = glm::mat3(
            glm::vec3(ySqr + zSqr, 0, 0),
            glm::vec3(0, xSqr + zSqr, 0),
            glm::vec3(0, 0, xSqr + ySqr)
        );

        m_inertiaTensorIntegral = (m_mass / 12.0f) * inertiaMat;
        m_inverseInertiaTensorIntegral = glm::inverse(m_inertiaTensorIntegral);
    }

    /*
     * Mass of the body, measured in Kg.
     */
    float m_mass;

    bool m_gravity;

    /*
     * Inertia Tensor:
     * Describes the body density throughout the object
     * which describes how hard something is to rotate.
     */
    glm::mat3 m_inertiaTensorIntegral;
    glm::mat3 m_inverseInertiaTensorIntegral;

    /*
     * Linear Momentum Calculated From:
     * P(t) = Mv(t)
     * Linear Momentum is the combined sum of all forces acting on the rigid body.
     * To get the current velocity, divide by the bodies mass.
     */
    glm::vec3 m_linearMomentum;

    /*
     * Angular Momentum Calculated From:
     * L(t) = I(t)w(t)
     * I(t) is the Inertia Tensor
     * w(t) is the rotation to be applied.
     */
    glm::vec3 m_angularMomentum;

    POD_Transform& m_transform;
};
