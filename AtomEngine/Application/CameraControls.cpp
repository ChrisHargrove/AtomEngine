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
        Transform* cam = GetComponent<Transform>();
        cam->Translate(cam->GetForward());
    }
    if (Input::Instance()->IsKeyHeld(SDLK_s)) {
        Transform* cam = GetComponent<Transform>();
        cam->Translate(-cam->GetForward());
    }
    if (Input::Instance()->IsKeyHeld(SDLK_a)) {
        Transform* cam = GetComponent<Transform>();
        cam->Translate(cam->GetRight());
    }
    if (Input::Instance()->IsKeyHeld(SDLK_d)) {
        Transform* cam = GetComponent<Transform>();
        cam->Translate(-cam->GetRight());
    }
    if (Input::Instance()->IsKeyHeld(SDLK_SPACE)) {
        Transform* cam = GetComponent<Transform>();
        cam->Translate(cam->GetUp());
    }
    if (Input::Instance()->IsKeyHeld(SDLK_LSHIFT)) {
        Transform* cam = GetComponent<Transform>();
        cam->Translate(-cam->GetUp());
    }
    if (Input::Instance()->HasMouseMoved()) {
        Transform* cam = GetComponent<Transform>();
        glm::vec2 rot = glm::vec2(Input::Instance()->GetMouseMove().m_relativeX * deltaTime, Input::Instance()->GetMouseMove().m_relativeY * deltaTime);
        cam->Rotate(glm::vec3(rot.x, -rot.y, 0.0));
    }
}

void CameraControls::Initialize()
{
}
