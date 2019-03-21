#pragma once
#include "ECS_Component.h"
#include "POD_Transform.h"

struct TransformComponent : public ECSComponent<TransformComponent>
{
    POD_Transform m_transform;
};
