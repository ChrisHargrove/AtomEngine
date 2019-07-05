#include "Transform.h"

#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/matrix_access.hpp>
#include <GLM/gtx/quaternion.hpp>


Transform::Transform()
{
    m_position = glm::vec3(0, 0, 0);
    m_orientation = glm::quat();
    m_scale = glm::vec3(1, 1, 1);
    m_transformMatrix = glm::mat4(1.0f);
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

bool Transform::Initialize()
{
    m_transform = this;
    return true;
}

void Transform::CalculateTransform()
{
    m_transformMatrix = glm::scale(glm::mat4(1.0f), m_scale);
    m_transformMatrix = glm::translate(m_transformMatrix, m_position);
    m_orientation = glm::normalize(m_orientation);
    m_transformMatrix *= glm::mat4_cast(m_orientation);
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
    m_orientation = rotation;
    CalculateTransform();
}

void Transform::SetRotation(glm::quat rotation)
{
    m_orientation = rotation;
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
    m_orientation = glm::normalize(m_orientation);
    return m_orientation;
}

glm::mat3 Transform::GetRotationMatrix()
{
    return glm::mat3(m_transformMatrix);
}

glm::vec3& Transform::GetEulerAngles()
{
    return m_eulerAngles;
}

glm::vec3& Transform::GetScale()
{
    CalculateTransform();
    return m_scale;
}

glm::mat4& Transform::GetTransform()
{
    CalculateTransform();
    return m_transformMatrix;
}

glm::mat4* Transform::GetTransformPtr()
{
    CalculateTransform();
    return &m_transformMatrix;
}

glm::vec3 Transform::GetForward()
{
    CalculateTransform();
    return glm::row(m_transformMatrix, 2);
}

glm::vec3 Transform::GetRight()
{
    CalculateTransform();
    return glm::row(m_transformMatrix, 0);
}

glm::vec3 Transform::GetUp()
{
    CalculateTransform();
    return glm::row(m_transformMatrix, 1);
}

void Transform::Rotate(glm::vec3 rotation)
{
    m_eulerAngles.x += glm::degrees(rotation.y);
    m_orientation = glm::rotate(m_orientation, rotation.x, glm::vec3(0.0, 1.0, 0.0));
    CalculateTransform();

    m_eulerAngles.y += glm::degrees(rotation.x);
    m_orientation = glm::rotate(m_orientation, rotation.y, GetRight());
    CalculateTransform();

    m_eulerAngles.z += glm::degrees(rotation.z);
    m_orientation = glm::rotate(m_orientation, rotation.z, GetForward());
    CalculateTransform();

}

void Transform::Translate(glm::vec3 translation)
{
    m_position += translation;
    m_transformMatrix = translate(m_transformMatrix, translation);
}

void Transform::Scale(glm::vec3 scale)
{
    m_scale += scale;
    m_transformMatrix = glm::scale(m_transformMatrix, scale);
}
