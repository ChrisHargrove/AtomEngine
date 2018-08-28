#pragma once

#include <GLM/glm.hpp>

class AABB
{
public:
    AABB();
    AABB(glm::vec3 position, glm::vec3 size);
    ~AABB();

    void Recalculate(glm::vec3 position, glm::vec3 size);

    void DebugDraw();

private:
    glm::vec3 m_position;
    glm::vec3 m_extents;
};

