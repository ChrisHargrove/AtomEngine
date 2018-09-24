#pragma once

#include "State.h"

#include "GameObject.h"
#include <list>
#include <GLM/glm.hpp>

#include "Buffer.h"
#include "Camera.h"
#include "Quad.h"

class LoadState : public State
{
public:
    LoadState();
    ~LoadState();

    // Inherited via State
    virtual bool Initialize() override;
    virtual void Input() override;
    virtual void Update(float delta) override;
    virtual void Render() override;
    virtual bool Shutdown() override;
    virtual void Pause() override;
    virtual void Resume() override;
    virtual bool IsPaused() override;

    std::list<GameObject*> GameObjectList;

    Camera* m_mainCamera;

    glm::vec3 rotation;

    Quad* screenFrame;

    FrameBuffer frameBuffer;
    bool _wireframe = false;

    UniformBuffer testUni;
};

