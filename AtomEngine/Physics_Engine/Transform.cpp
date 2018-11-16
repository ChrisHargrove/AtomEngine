#include "Transform.h"

#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/matrix_access.hpp>
#include <GLM/gtx/quaternion.hpp>


Transform::Transform()
{
    m_position = glm::vec3(0, 0, 0);
    m_rotation = glm::quat();
    m_scale = glm::vec3(1, 1, 1);
    m_tranformMatrix = glm::mat4(1.0f);
    m_name = "Transform";
    m_typeInfo = &typeid(this);
}


Transform::~Transform()
{
}

void Transform::Update(float deltaTime)
{
    CalculateRotation();
    CalculateTransform();
}

void Transform::Initialize()
{
}

void Transform::CalculateTransform()
{
    m_tranformMatrix = glm::translate(glm::scale(glm::mat4(1.0f), m_scale) * glm::mat4_cast(m_rotation), m_position);
}

void Transform::CalculateRotation()
{
    
}

void Transform::SetPosition(glm::vec3 translation)
{
    m_position = translation;
    CalculateTransform();
}

void Transform::SetRotation(glm::vec3 rotation)
{
    m_rotation = rotation;
    CalculateTransform();
}

void Transform::SetRotation(glm::quat rotation)
{
    m_rotation = rotation;
    CalculateTransform();
}

void Transform::SetScale(glm::vec3 scale)
{
    m_scale = scale;
    CalculateTransform();
}

glm::vec3& Transform::GetPosition()
{
    return m_position;
}

glm::quat& Transform::GetRotation()
{
    return m_rotation;
}

glm::vec3& Transform::GetEulerAngles()
{
    return m_eulerAngles;
}

glm::vec3& Transform::GetScale()
{
    return m_scale;
}

glm::mat4 Transform::GetTransform()
{
    CalculateTransform();
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

void Transform::Rotate(glm::vec3 rotation)
{
    m_eulerAngles.x += glm::degrees(rotation.y);
    m_rotation = glm::rotate(m_rotation, rotation.x, glm::vec3(0.0, 1.0, 0.0));
    CalculateTransform();

    m_eulerAngles.y += glm::degrees(rotation.x);
    m_rotation = glm::rotate(m_rotation, rotation.y, GetRight());
    CalculateTransform();

    m_eulerAngles.z += glm::degrees(rotation.z);
    m_rotation = glm::rotate(m_rotation, rotation.z, GetForward());
    CalculateTransform();

}

void Transform::Translate(glm::vec3 translation)
{
    m_position += translation;
}

void Transform::Scale(glm::vec3 scale)
{
    m_scale += scale;
}
