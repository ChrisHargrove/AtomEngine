#pragma once

#include <array>
#include <GLM/glm.hpp>

#include "POD_Transform.h"
#include "POD_Mesh.h"
#include "POD_RigidBody.h"

#include "Utilities.h";

class AABB
{
public:
    enum CollisionStatus
    {
        NO_COLLISION = 0,
        POTENTIAL,
        COLLIDING
    };

    AABB(POD_Mesh* meshIn)
    {
        meshIn->GetBounds(m_minBounds, m_maxBounds);

        m_vertices[0] = m_minBounds;
        m_vertices[1] = glm::vec3(m_maxBounds.x, m_minBounds.y, m_minBounds.z);
        m_vertices[2] = glm::vec3(m_minBounds.x, m_maxBounds.y, m_minBounds.z);
        m_vertices[3] = glm::vec3(m_maxBounds.x, m_maxBounds.y, m_minBounds.z);
        m_vertices[4] = glm::vec3(m_minBounds.x, m_minBounds.y, m_maxBounds.z);
        m_vertices[5] = glm::vec3(m_maxBounds.x, m_minBounds.y, m_maxBounds.z);
        m_vertices[6] = glm::vec3(m_minBounds.x, m_maxBounds.y, m_maxBounds.z);
        m_vertices[7] = m_maxBounds;
    }

    AABB(glm::vec3 minimum, glm::vec3 maximum)
    {
        m_minBounds = minimum;
        m_maxBounds = maximum;

        m_vertices[0] = m_minBounds;
        m_vertices[1] = glm::vec3(m_maxBounds.x, m_minBounds.y, m_minBounds.z);
        m_vertices[2] = glm::vec3(m_minBounds.x, m_maxBounds.y, m_minBounds.z);
        m_vertices[3] = glm::vec3(m_maxBounds.x, m_maxBounds.y, m_minBounds.z);
        m_vertices[4] = glm::vec3(m_minBounds.x, m_minBounds.y, m_maxBounds.z);
        m_vertices[5] = glm::vec3(m_maxBounds.x, m_minBounds.y, m_maxBounds.z);
        m_vertices[6] = glm::vec3(m_minBounds.x, m_maxBounds.y, m_maxBounds.z);
        m_vertices[7] = m_maxBounds;
    }

    AABB()
    {
        m_minBounds = glm::vec3(-1.0f);
        m_maxBounds = glm::vec3(1.0f);

        m_vertices[0] = m_minBounds;
        m_vertices[1] = glm::vec3(m_maxBounds.x, m_minBounds.y, m_minBounds.z);
        m_vertices[2] = glm::vec3(m_minBounds.x, m_maxBounds.y, m_minBounds.z);
        m_vertices[3] = glm::vec3(m_maxBounds.x, m_maxBounds.y, m_minBounds.z);
        m_vertices[4] = glm::vec3(m_minBounds.x, m_minBounds.y, m_maxBounds.z);
        m_vertices[5] = glm::vec3(m_maxBounds.x, m_minBounds.y, m_maxBounds.z);
        m_vertices[6] = glm::vec3(m_minBounds.x, m_maxBounds.y, m_maxBounds.z);
        m_vertices[7] = m_maxBounds;
    }

    ~AABB(){}

    inline void RecalculateAABB(POD_Transform* transform, POD_Mesh* mesh) {
        auto vertices = m_vertices;

        m_transform = transform;
        m_mesh = mesh;

        RecalculateOBB(vertices, transform);

        m_minBounds.x = m_maxBounds.x = vertices[0].x;
        m_minBounds.y = m_maxBounds.y = vertices[0].y;
        m_minBounds.z = m_maxBounds.z = vertices[0].z;

        for (int i = 0; i < vertices.size(); i++) {

            if (vertices[i].x > m_maxBounds.x) m_maxBounds.x = vertices[i].x;
            if (vertices[i].y > m_maxBounds.y) m_maxBounds.y = vertices[i].y;
            if (vertices[i].z > m_maxBounds.z) m_maxBounds.z = vertices[i].z;

            if (vertices[i].x < m_minBounds.x) m_minBounds.x = vertices[i].x;
            if (vertices[i].y < m_minBounds.y) m_minBounds.y = vertices[i].y;
            if (vertices[i].z < m_minBounds.z) m_minBounds.z = vertices[i].z;
        }
    }

    glm::vec3 GetExtents() {
        return (m_maxBounds - m_minBounds);
    }

    float GetVolume() {
        glm::vec3 extents = GetExtents();
        return extents.x * extents.y * extents.z;
    }

    bool Collides(AABB* other) {
        return ((other->m_minBounds.y <= m_maxBounds.y && other->m_maxBounds.y >= m_minBounds.y) &&
            (other->m_minBounds.x <= m_maxBounds.x && other->m_maxBounds.x >= m_minBounds.x) &&
            (other->m_minBounds.z <= m_maxBounds.z && other->m_maxBounds.z >= m_minBounds.z));
    }

    bool Contains(const glm::vec3& point) {
        return ((point.x >= m_minBounds.x && point.x <= m_maxBounds.x) &&
            (point.y >= m_minBounds.y && point.y <= m_maxBounds.y) &&
            (point.z >= m_minBounds.z && point.z <= m_maxBounds.z));
    }

    bool Contains(const AABB& aabb) {
        return ((aabb.m_minBounds >= m_minBounds && aabb.m_maxBounds <= m_maxBounds));
    }

    CollisionStatus& IsColliding() {
        return m_collisionStatus;
    }

    static AABB MergeAABB(const AABB& a, const AABB& b) {
        glm::vec3 min;
        glm::vec3 max;
        for(int i = 0; i < 3; i++) {
            min[i] = (std::min)(a.m_minBounds[i], b.m_minBounds[i]);
            max[i] = (std::max)(a.m_maxBounds[i], b.m_maxBounds[i]);
        }

        return AABB(min, max);
    }

protected:

    friend class BoundingVolumeHeirarchy;
    friend class BVHNode;
    friend class GJK;

    inline void RecalculateOBB(std::array<glm::vec3, 8>& vertices, POD_Transform* transform) {
        auto rotation = transform->GetRotationMatrix();
        auto scale = transform->GetScale();
        auto translation = transform->GetPosition();
        for(auto& vert : vertices) {
            vert *= scale;
            vert = rotation * vert;
            vert += translation;
        }
    }

    CollisionStatus m_collisionStatus = NO_COLLISION;

    std::array<glm::vec3, 8> m_vertices;

    glm::vec3 m_minBounds;
    glm::vec3 m_maxBounds;

    void* m_containingNode;
    POD_Transform* m_transform;
    POD_Mesh* m_mesh;
};
