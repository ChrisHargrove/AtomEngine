#pragma once
#include "ECS_Component.h"
#include "POD_Mesh.h"

struct MeshComponent : public ECSComponent<MeshComponent>
{
    POD_Mesh m_mesh;
};
