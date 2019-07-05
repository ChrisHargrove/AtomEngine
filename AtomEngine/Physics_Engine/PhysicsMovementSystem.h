#pragma once
#include "ECS_System.h"
#include "RigidBodyComponent.h"
#include "TransformComponent.h"
#include "ProfilerManager.h"

class PhysicsMovementSystem : public BaseECSSystem
{
public:
    PhysicsMovementSystem() : BaseECSSystem()
    {
        AddComponentType(TransformComponent::ID);
        AddComponentType(RigidBodyComponent::ID);
    }

    virtual void UpdateComponents(float deltaTime, std::vector<std::vector<BaseECSComponent*>>& componentArrays) override
    {
        Profiler::Instance()->Start("Update All Rigid Bodies");
        for (uint32_t i = 0; i < componentArrays[0].size(); i++)
        {
            POD_Transform& transform = ((TransformComponent*)componentArrays[0][i])->m_transform;
            POD_RigidBody& body = ((RigidBodyComponent*)componentArrays[1][i])->m_rigidBody;

            if(body.UsesGravity()) {
                body.ApplyForce(glm::vec3(0, (-9.81f * body.m_mass), 0) * deltaTime);
            }

            auto velocity = (body.m_linearMomentum / body.m_mass) * deltaTime;

            transform.Translate(velocity);

            auto rotationMatrix = transform.GetRotationMatrix();

            auto inverseInertia = rotationMatrix * body.m_inverseInertiaTensorIntegral * glm::transpose(rotationMatrix);

            auto angularVelocity = (inverseInertia * body.m_angularMomentum) * deltaTime;

            auto q = glm::quat(0, angularVelocity.x, angularVelocity.y, angularVelocity.z);

            auto spin = 0.5f * (q * transform.GetRotation());

            transform.SetRotation(transform.GetRotation() + spin);
        }
        Profiler::Instance()->End("Update All Rigid Bodies");
    }

};
