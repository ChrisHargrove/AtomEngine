#pragma once

#include <GLM/glm.hpp>
#include <GLM/mat4x4.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/quaternion.hpp>
#include <GLM/gtx/euler_angles.hpp>
#include <GLM/gtc/matrix_access.hpp>

class POD_Transform
{
public:
    inline POD_Transform() :
        m_position(glm::vec3(0, 0, 0)),
        m_scale(glm::vec3(1, 1, 1)),
        m_matrix(glm::mat4(1.0f))
    {
        GetMatrix();
    }

    ~POD_Transform() = default;

    void SetPosition(const glm::vec3& position) {
        m_position = position;
        needsUpdate = true;
    }

    void SetRotation(const glm::quat& rotation) {
        m_orientation = rotation;
        needsUpdate = true;
    }

    void SetScale(const glm::vec3& scale) {
        m_scale = scale;
        needsUpdate = true;
    }

    glm::mat4 GetMatrix() {
        if(needsUpdate) {
            auto translation = glm::translate(glm::mat4(1.0f), m_position);
            m_orientation = glm::normalize(m_orientation);
            auto rotation = glm::mat4_cast(m_orientation);
            auto scale = glm::scale(glm::mat4(1.0f), m_scale);
            m_matrix = translation * rotation * scale;
            needsUpdate = false;
        }
        
        return m_matrix;

        /*glm::mat4 m_matrix = glm::scale(glm::mat4(1.0f), m_scale);
        m_matrix = glm::translate(m_matrix, m_position);
        m_orientation = glm::normalize(m_orientation);
        m_matrix *= glm::mat4_cast(m_orientation);
        return m_matrix;*/
    }

    glm::vec3 GetPosition() {
        return m_position;
    }

    glm::vec3 GetLocalPosition() {
        return WorldToLocal(m_position);
    }

    glm::quat GetRotation() {
        return m_orientation;
    }

    glm::mat3 GetRotationMatrix() {
        return glm::mat3_cast(m_orientation);
    }

    glm::vec3 GetScale() {
        return m_scale;
    }

    void Translate(const glm::vec3& translation) {
        m_position += translation;
        needsUpdate = true;
    }

    void Rotate(const glm::vec3& angles) {

        m_orientation = glm::rotate(m_orientation, angles.x, glm::vec3(0.0, 1.0, 0.0));
        auto rotation = glm::mat4_cast(m_orientation);
        m_orientation = glm::rotate(m_orientation, angles.y, glm::vec3(glm::row(rotation, 0)));
        rotation = glm::mat4_cast(m_orientation);
        m_orientation = glm::rotate(m_orientation, angles.z, glm::vec3(glm::row(rotation, 2)));
        needsUpdate = true;
    }

    void Rotate(const glm::quat& rotation) {
        m_orientation = rotation * m_orientation;
        needsUpdate = true;
    }

    void Scale(const glm::vec3& scale) {
        m_scale += scale;
        needsUpdate = true;
    }

    glm::vec3 GetRight() {
        return glm::row(glm::mat3_cast(m_orientation), 0);
    }

    glm::vec3 GetUp() {
        return glm::row(glm::mat3_cast(m_orientation), 1);
    }

    glm::vec3 GetForward() {
        return glm::row(glm::mat3_cast(m_orientation), 2);
    }

    glm::vec3 WorldToLocal(const glm::vec3& vector)
    {
        return glm::transpose(glm::inverse(glm::mat3(GetMatrix()))) * vector;
    }

    glm::vec3 LocalToWorld(const glm::vec3& vector)
    {
        return glm::transpose(glm::mat3(GetMatrix())) * vector;
    }

private:
    glm::vec3 m_position;
    glm::vec3 m_scale;
    glm::quat m_orientation;

    glm::mat4 m_matrix;

    bool needsUpdate = true;
};

