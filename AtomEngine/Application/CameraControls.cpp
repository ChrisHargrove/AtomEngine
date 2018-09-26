#include "CameraControls.h"
#include "Transform.h"
#include "SDL/SDL.h"

#include "InputManager.h"

CameraControls::CameraControls()
{
}


CameraControls::~CameraControls()
{
}

void CameraControls::Update(float deltaTime)
{
    if (Input::Instance()->IsKeyHeld(SDLK_w)) {
        _transform->Translate(_transform->GetForward());
    }
    if (Input::Instance()->IsKeyHeld(SDLK_s)) {
        _transform->Translate(-_transform->GetForward());
    }
    if (Input::Instance()->IsKeyHeld(SDLK_a)) {
        _transform->Translate(_transform->GetRight());
    }
    if (Input::Instance()->IsKeyHeld(SDLK_d)) {
        _transform->Translate(-_transform->GetRight());
    }
    if (Input::Instance()->IsKeyHeld(SDLK_SPACE)) {
        _transform->Translate(glm::vec3(0, 1, 0));
    }
    if (Input::Instance()->IsKeyHeld(SDLK_LSHIFT)) {
        _transform->Translate(-glm::vec3(0, 1, 0));
    }
    if (Input::Instance()->HasMouseMoved()) {
        glm::vec2 rot = glm::vec2(Input::Instance()->GetMouseMove().m_relativeX * deltaTime, Input::Instance()->GetMouseMove().m_relativeY * deltaTime);
        _transform->Rotate(glm::vec3(rot.x, -rot.y, 0.0));
    }
}

void CameraControls::Initialize()
{
    _transform = GetComponent<Transform>().get();
}
