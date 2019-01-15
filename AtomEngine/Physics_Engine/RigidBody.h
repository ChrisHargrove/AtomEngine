#pragma once
#include "Component.h"

class PhysicsManager;

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

class ATOM_API RigidBody : public Component
{
public:
    friend class PhysicsManager;

    RigidBody();
    virtual ~RigidBody();

    virtual bool Initialize() override;
    void Simulate(float deltaTime);

    void ApplyForce(glm::vec3 force);
    void ApplyForce(float x, float y, float z);
    void ApplyForce(glm::vec3 force, glm::vec3 position);

    void ApplyTorque(glm::vec3 torque);

    float& GetMass();
    void SetMass(float mass);

    bool& UsesGravity();

    void CalculateInertiaTensorIntegral();
    void CalculateInertiaTensorIntegral(glm::vec3 dimensions);


protected:
    glm::vec3 m_dimensions;

    bool m_affectedByGravity;

    /*
     * Mass of the body, measured in Kg.
     */
    float m_mass;

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

private:
    friend class cereal::access;

    template<class Archive>
    void serialize(Archive &archive) {
        archive(
            cereal::make_nvp("Dimensions", m_dimensions),
            cereal::make_nvp("Mass", m_mass),
            cereal::make_nvp("Gravity", m_affectedByGravity),
            cereal::make_nvp("LinearMomentum", m_linearMomentum),
            cereal::make_nvp("AngularMomentum", m_angularMomentum));
    }

};

CEREAL_REGISTER_TYPE_WITH_NAME(RigidBody, "RigidBody");
