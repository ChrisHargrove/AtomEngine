#include "Transform.h"

#include <GLM/gtc/quaternion.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/matrix_access.hpp>


Transform::Transform()
{
    m_position = glm::vec3(0, 0, 0);
    m_rotation = glm::vec3(0, 0, 0);
    m_scale = glm::vec3(1, 1, 1);
    m_tranformMatrix = glm::mat4(1.0f);
}


Transform::~Transform()
{
}

void Transform::Update(float deltaTime)
{
    m_tranformMatrix = glm::translate(glm::scale(glm::mat4(1.0f), m_scale) * glm::mat4_cast(glm::quat(m_rotation)), m_position);
}

void Transform::Initialize()
{
}

void Transform::SetPosition(glm::vec3 translation)
{
    m_position = translation;
}

void Transform::SetRotation(glm::vec3 rotation)
{
    m_rotation = rotation;
}

void Transform::SetScale(glm::vec3 scale)
{
    m_scale = scale;
}

glm::vec3 Transform::GetPosition()
{
    return m_position;
}

glm::vec3 Transform::GetRotation()
{
    return m_rotation;
}

glm::vec3 Transform::GetScale()
{
    return m_scale;
}

glm::mat4 Transform::GetTransform()
{
    return m_tranformMatrix;
}

glm::vec3 Transform::GetForward()
{
    return glm::row(m_tranformMatrix, 2);
}

glm::vec3 Transform::GetRight()
{
    return glm::row(m_tranformMatrix, 0);
}

glm::vec3 Transform::GetUp()
{
    return glm::row(m_tranformMatrix, 1);
}
