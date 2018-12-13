#include "CameraControls.h"
#include "SDL/SDL.h"

#include "InputManager.h"
#include "ScreenManager.h"

#include "Camera.h"
#include "LogManager.h"

#include <GLM/gtx/vector_angle.hpp>

CameraControls::CameraControls()
{
    m_name = "CameraControls";
    m_typeInfo = &typeid(this);
}

CameraControls::~CameraControls()
{
}

bool CameraControls::Initialize()
{
    _transform = GetComponent<Transform>();
    _mouseCaptured = Input::Instance()->IsMouseCaptured();
    if (_transform != nullptr) return true;

    return false;
}

void CameraControls::Update(float deltaTime)
{
    auto camera = GetComponent<Camera>();

    if(_mouseCaptured)
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

            if (camera != nullptr)
            {
                if (camera->IsPitchConstrained()) {
                    auto front = glm::cross(_transform->GetRight(), glm::vec3(0, 1, 0));
                    auto pitch = glm::degrees(glm::angle(front, _transform->GetForward()));

                    if (pitch > 85.0f)
                    {
                        _transform->Rotate(glm::vec3(0, rot.y, 0));
                    }
                    else if (pitch < -85.0f)
                    {
                        _transform->Rotate(glm::vec3(0, rot.y, 0));
                    }
                }
            }

        }
    }
    
    if (Input::Instance()->IsKeyPressed(SDLK_LALT)) {
        _mouseCaptured = !_mouseCaptured;
        Input::Instance()->CaptureMouse(_mouseCaptured);
    }
}
