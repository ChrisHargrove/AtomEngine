#pragma once

#include <State.h>
#include "Buffer.h"
#include "Quad.h"
#include "Scene.h"
#include "POD_Transform.h"
#include "ECS_Manager.h"
#include "Renderer.h"
#include "RenderMeshSystem.h"

class NewECS : public State
{
public:
    NewECS();
    virtual ~NewECS();

    bool Initialize() override;
    void Input() override;
    void Update(float delta) override;
    void Render() override;
    bool Shutdown() override;
    void Pause() override;
    void Resume() override;
    bool IsPaused() override;

    Quad* m_screenFrame;

    FrameBuffer m_frameBuffer;
    bool m_wireFrame = false;
    bool m_showDebug = false;

    std::shared_ptr<Skybox> m_skybox;
    UniformBuffer m_uniformBuffer;

    //test.
    ECS_Manager m_ecs;
    Renderer m_renderer;
    RenderMeshSystem m_renderMeshSystem;
    ECSSystemList m_renderPipeline;


    POD_Transform* camTransform;

    glm::mat4 view;
    float viewFloat;
};

