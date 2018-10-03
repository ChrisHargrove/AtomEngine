#include "CameraControls.h"
#include "Transform.h"
#include "SDL/SDL.h"

#include "InputManager.h"
#include "ScreenManager.h"

CameraControls::CameraControls()
{
    m_name = "CameraControls";
}

CameraControls::~CameraControls()
{
}

void CameraControls::Initialize()
{
    _transform = GetComponent<Transform>();
    _mouseCaptured = Input::Instance()->IsMouseCaptured();
}

void CameraControls::Update(float deltaTime)
{
    auto transform = (Transform*)_transform;

    if (Input::Instance()->IsKeyHeld(SDLK_w)) {
        transform->Translate(transform->GetForward());
    }
    if (Input::Instance()->IsKeyHeld(SDLK_s)) {
        transform->Translate(-transform->GetForward());
    }
    if (Input::Instance()->IsKeyHeld(SDLK_a)) {
        transform->Translate(transform->GetRight());
    }
    if (Input::Instance()->IsKeyHeld(SDLK_d)) {
        transform->Translate(-transform->GetRight());
    }
    if (Input::Instance()->IsKeyHeld(SDLK_SPACE)) {
        transform->Translate(glm::vec3(0, 1, 0));
    }
    if (Input::Instance()->IsKeyHeld(SDLK_LSHIFT)) {
        transform->Translate(-glm::vec3(0, 1, 0));
    }
    if (Input::Instance()->HasMouseMoved() && _mouseCaptured) {
        glm::vec2 rot = glm::vec2(Input::Instance()->GetMouseMove().m_relativeX * deltaTime, Input::Instance()->GetMouseMove().m_relativeY * deltaTime);
        transform->Rotate(glm::vec3(rot.x, -rot.y, 0.0));
    }
    if (Input::Instance()->IsKeyPressed(SDLK_LALT)) {
        _mouseCaptured = !_mouseCaptured;
        Input::Instance()->CaptureMouse(_mouseCaptured);
    }
}
