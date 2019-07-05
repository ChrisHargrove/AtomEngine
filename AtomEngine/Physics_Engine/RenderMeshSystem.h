#pragma once
#include "ECS_System.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "InstancedRenderer.h"
#include "LogManager.h"

class RenderMeshSystem : public BaseECSSystem
{
public:
    RenderMeshSystem(InstancedRenderer& rendererIn) : 
        BaseECSSystem(), 
        m_renderer(rendererIn)
    {
        AddComponentType(TransformComponent::ID);
        AddComponentType(MeshComponent::ID);
    }
    
    virtual void UpdateComponents(float deltaTime, std::vector<std::vector<BaseECSComponent*>>& componentArrays) override
    {
        for(uint32_t i = 0; i < componentArrays[0].size(); i++)
        {
            TransformComponent* transform = (TransformComponent*)componentArrays[0][i];
            MeshComponent* mesh = (MeshComponent*)componentArrays[1][i];

            m_renderer.AddToBuffer(mesh->m_mesh.GetSubmeshList(), transform->m_transform.GetMatrix());
        }
    }

private:
    InstancedRenderer& m_renderer;
};
