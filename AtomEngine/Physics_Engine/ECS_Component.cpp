#include "ECS_Component.h"

std::vector<ComponentType>* BaseECSComponent::m_componentTypes;

uint32_t BaseECSComponent::RegisterComponentType(ECSComponentCreateFunction createFunc, ECSComponentFreeFunction freeFunc, size_t size)
{
    //Lazy initialization just in case the compiler doesn't follow correct static variable initialization.
    if(m_componentTypes == nullptr) {
        m_componentTypes = new std::vector<ComponentType>();
    }
    //Set the component ID to be the current size of the vector.
    const uint32_t componentID = m_componentTypes->size();
    //Use emplace to call the constructor for wrapped tuple type.
    m_componentTypes->emplace_back(createFunc, freeFunc, size);
    //return the ID of the newly registered component.
    return componentID;
}
