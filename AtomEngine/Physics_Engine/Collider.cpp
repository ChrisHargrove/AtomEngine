#include "Collider.h"



Collider::Collider()
{
}


Collider::~Collider()
{
}

glm::vec3 Collider::GetMinimumBounds() const
{
    return m_minBounds;
}

glm::vec3 Collider::GetMaximumBounds() const
{
    return m_maxBounds;
}
