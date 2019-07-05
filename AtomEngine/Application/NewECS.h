#pragma once

#include <State.h>
#include "Buffer.h"
#include "Quad.h"
#include "POD_Transform.h"
#include "ECS_Manager.h"
#include "InstancedRenderer.h"
#include "RenderMeshSystem.h"
#include "PostRenderer.h"
#include "SkyboxRenderer.h"
#include "PhysicsMovementSystem.h"
#include "DebugRenderer.h"
#include "RenderDebugSystem.h"
#include "CollisionDetectionSystem.h"

class NewECS : public State
{
public:
    NewECS();
    virtual ~NewECS();
    void CreateSimulation();

    bool Initialize() override;
    void Input() override;
    void Update(float delta) override;
    void Render() override;
    bool Shutdown() override;
    void Pause() override;
    void Resume() override;
    bool IsPaused() override;

    bool m_wireFrame = false;
    bool m_showDebug = false;

    //test.
    ECS_Manager m_ecs;

    PostRenderer m_mainRenderer;
    SkyboxRenderer m_skyboxRenderer;
    InstancedRenderer m_instanceRenderer;
    DebugRenderer m_debugRenderer;

    RenderMeshSystem m_renderMeshSystem;
    RenderDebugSystem m_renderDebugSystem;
    PhysicsMovementSystem m_physicsMovementSystem;
    CollisionDetectionSystem m_collisionDetection;

    ECSSystemList m_renderPipeline;
    ECSSystemList m_physicsSystems;

    POD_Transform* camTransform;
};

