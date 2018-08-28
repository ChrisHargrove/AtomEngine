#include "AABB.h"



AABB::AABB() :
    m_position(glm::vec3(0, 0, 0)),
    m_extents(glm::vec3(0, 0, 0))
{
}

AABB::AABB(glm::vec3 position, glm::vec3 size) :
    m_position(position),
    m_extents(size * 0.5f)
{
}

AABB::~AABB()
{
}

void AABB::Recalculate(glm::vec3 position, glm::vec3 size)
{
    m_position = position;
    m_extents = size * 0.5f;
}

void AABB::DebugDraw()
{
    //TODO: Create a debug function to draw the lines of the AABB.
}
