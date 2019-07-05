#pragma once
#include "ECS_Component.h"
#include "POD_RigidBody.h"

struct RigidBodyComponent : ECSComponent<RigidBodyComponent>
{
    RigidBodyComponent(POD_Transform& transformIn) :
        m_rigidBody(POD_RigidBody(transformIn)) {}

    POD_RigidBody m_rigidBody;
};
