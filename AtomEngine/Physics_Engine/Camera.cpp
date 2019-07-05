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
    m_typeInfo = &typeid(this);
}

Camera::~Camera()
{
}

bool Camera::Initialize()
{
    m_transform = GetComponent<Transform>();
    return m_transform != nullptr;
}


void Camera::Update(float deltaTime)
{

}

float Camera::GetZoom() const
{
    return m_zoom;
}

glm::mat4 Camera::GetViewMatrix()
{
    if (m_transform == nullptr) return glm::mat4();

    m_viewMatrix = glm::lookAt(m_transform->GetPosition(), m_transform->GetPosition() + m_transform->GetForward(), m_transform->GetUp());
    return m_viewMatrix;
}

glm::mat4 Camera::GetOrthoMatrix() const
{
    if (m_transform == nullptr) return glm::mat4();

    return glm::lookAt(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, -1.0), m_transform->GetUp());
}

void Camera::EnablePitchConstraint(bool enable)
{
    m_isPitchConstrained = enable;
}

void Camera::SetTarget(glm::vec3 target)
{
    m_target = target;
}

bool Camera::IsPitchConstrained() const
{
    return m_isPitchConstrained;;
}

