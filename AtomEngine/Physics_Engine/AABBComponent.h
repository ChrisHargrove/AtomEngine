#pragma once
#include "ECS_Component.h"
#include "AABB.h"

struct AABBComponent : public ECSComponent<AABBComponent>
{
    AABBComponent(POD_Mesh* meshIn) :
        m_aabb(AABB(meshIn)) { }

    AABB m_aabb;
};
