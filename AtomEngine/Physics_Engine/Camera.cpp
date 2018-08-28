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
}

Camera::~Camera()
{
}

void Camera::Initialize()
{
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
    Transform* transform = m_parent->GetComponent<Transform>();
    if (transform == nullptr) return glm::mat4();
    m_viewMatrix = glm::lookAt(transform->GetPosition(), transform->GetPosition() + transform->GetForward(), transform->GetUp());
    return m_viewMatrix;
}

glm::mat4 Camera::GetOrthoMatrix()
{
    Transform* transform = m_parent->GetComponent<Transform>();
    if (transform == nullptr) return glm::mat4();
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


