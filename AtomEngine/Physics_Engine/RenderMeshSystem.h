#pragma once
#include "ECS_System.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "Renderer.h"

class RenderMeshSystem : public BaseECSSystem
{
public:
    RenderMeshSystem(Renderer& rendererIn) : BaseECSSystem(), renderer(rendererIn) {
        AddComponentType(TransformComponent::ID);
        AddComponentType(MeshComponent::ID);
    }

    virtual void UpdateComponents(float deltaTime, BaseECSComponent** components) override
    {
        TransformComponent* transform = (TransformComponent*)components[0];
        MeshComponent* mesh = (MeshComponent*)components[1];

        renderer.AddToBuffer(mesh->m_mesh.GetSubmeshList(), transform->m_transform.GetMatrix());
    }

private:
    Renderer& renderer;
};
