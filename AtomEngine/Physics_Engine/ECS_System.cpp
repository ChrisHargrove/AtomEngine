#include "ECS_System.h"

bool ECSSystemList::RemoveSystem(BaseECSSystem& system)
{
    for (uint32_t i = 0; i < m_systems.size(); i++) {
        if (&system == m_systems[i]) {
            m_systems.erase(m_systems.begin() + i);
            return true;
        }
    }
    return false;
}
