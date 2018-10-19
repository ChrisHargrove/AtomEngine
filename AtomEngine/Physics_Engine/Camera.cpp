#include "Camera.h"
#include "GameObject.h"
#include "Transform.h"
#include <GLM\gtx\rotate_vector.hpp>

Camera::Camera() :
    m_zoom(0.0f),
    m_isPitchConstrained(true),
    m_target(glm::vec3(0,0,0)),
    m_viewMatrix(glm::mat4(1.0f))
{
    m_name = "Camera";
}

Camera::~Camera()
{
}

void Camera::Initialize()
{
    _transform = GetComponent<Transform>();
}

void Camera::Update(float deltaTime)
{

}

float Camera::GetZoom()
{
    return m_zoom;
}

glm::mat4 Camera::GetViewMatrix()
{
    if (_transform == nullptr) return glm::mat4();

    Transform* transform = static_cast<Transform*>(_transform);

    m_viewMatrix = glm::lookAt(transform->GetPosition(), transform->GetPosition() + transform->GetForward(), transform->GetUp());
    return m_viewMatrix;
}

glm::mat4 Camera::GetOrthoMatrix()
{
    if (_transform == nullptr) return glm::mat4();
    Transform* transform = static_cast<Transform*>(_transform);

    return glm::lookAt(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, -1.0), transform->GetUp());
}

void Camera::EnablePitchConstraint(bool enable)
{
    m_isPitchConstrained = enable;
}

void Camera::SetTarget(glm::vec3 target)
{
    m_target = target;
}

bool Camera::IsPitchConstrained()
{
    return m_isPitchConstrained;;
}

