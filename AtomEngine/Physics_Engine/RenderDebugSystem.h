#pragma once
#include "ECS_System.h"
#include "AABBComponent.h"
#include "DebugCuboid.h"
#include "DebugRenderer.h"
#include "TransformComponent.h"
#include "LogManager.h"

class RenderDebugSystem : public BaseECSSystem
{
public:
    RenderDebugSystem(DebugRenderer& rendererIn) :
        BaseECSSystem(),
        m_renderer(rendererIn)
    {
        AddComponentType(TransformComponent::ID);
        AddComponentType(AABBComponent::ID);

        m_debugCuboid.SetColor(glm::vec3(1, 0, 0));
    }

    bool* GetShowDebug() {
        return &m_shouldRender;
    }

    void UpdateComponents(float deltaTime, std::vector<std::vector<BaseECSComponent*>>& componentArrays) override
    {
        if (!m_shouldRender) return;

        for(auto& compArray : componentArrays) {
            if (compArray.size() == 0) return;
        }

        for (uint32_t i = 0; i < componentArrays[0].size(); i++)
        {
            POD_Transform transform = ((TransformComponent*)componentArrays[0][i])->m_transform;
            AABB& aabb = ((AABBComponent*)componentArrays[1][i])->m_aabb;

            glm::mat4 trans = glm::mat4(1.0f);
            trans = glm::translate(trans, transform.GetPosition());

            trans = glm::scale(trans, aabb.GetExtents());

            glm::vec3 color = glm::vec3(0.0f);

            switch(aabb.IsColliding())
            {
            case AABB::NO_COLLISION:
                color = glm::vec3(1.0f, 0.0f, 0.0f);
                break;
            case AABB::POTENTIAL: 
                color = glm::vec3(0.0f, 0.0f, 1.0f);
                break; 
            case AABB::COLLIDING: 
                color = glm::vec3(0.0f, 1.0f, 0.0f);
                break;
            default: ;
            }

            m_renderer.AddToBuffer(&m_debugCuboid, color, trans);
           
        }
    }

    

private:
    DebugRenderer& m_renderer;

    DebugCuboid m_debugCuboid;

    bool m_shouldRender = false;
};
