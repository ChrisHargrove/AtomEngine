#pragma once

#include "State.h"

#include "Buffer.h"
#include "Quad.h"

#include "Scene.h"
#include <memory>   

class LoadState : public State
{
public:
    LoadState();
    virtual ~LoadState();

    // Inherited via State
    virtual bool Initialize() override;
    virtual void Input() override;
    virtual void Update(float delta) override;
    virtual void Render() override;
    virtual bool Shutdown() override;
    virtual void Pause() override;
    virtual void Resume() override;
    virtual bool IsPaused() override;

    Quad* m_screenFrame;

    FrameBuffer m_frameBuffer;
    bool m_wireFrame = false;
    bool m_showDebug = false;

    std::shared_ptr<Scene> m_scene;
    std::vector<GameObject*> m_gameObjects;
};

