#include "ECS_Manager.h"
#include <algorithm>


ECS_Manager::~ECS_Manager()
{
    for (auto& component : m_components) {
        const auto typeSize = BaseECSComponent::GetTypeSize(component.first);
        auto freeFunc = BaseECSComponent::GetTypeFreeFunction(component.first);
        for (uint32_t i = 0; i < component.second.size(); i += typeSize) {
            freeFunc(reinterpret_cast<BaseECSComponent*>(&component.second[i]));
        }
    }

    for (auto& entity : m_entities) {
        delete entity;
    }
}

EntityHandle ECS_Manager::MakeEntity()
{
    const auto entity = new EntityRaw();
    const auto handle = static_cast<EntityHandle>(entity);

    entity->first = m_entities.size();
    m_entities.push_back(entity);
    return handle;
}

EntityHandle ECS_Manager::MakeEntity(BaseECSComponent** components, const uint32_t* componentIDs, size_t numComponents)
{
    
    const auto entity = new EntityRaw();
    const auto handle = static_cast<EntityHandle>(entity);

    for(uint32_t i = 0; i < numComponents; i++) {
        if (BaseECSComponent::IsValidType(componentIDs[i])) {
            delete entity;
            return nullptr;
        }
        AddComponentInternal(handle, entity->second, componentIDs[i], components[i]);
    }

    entity->first = m_entities.size();
    m_entities.push_back(entity);
    return handle;
}

void ECS_Manager::RemoveEntity(EntityHandle handle)
{
    auto entity = HandleToEntity(handle);
    for (auto& component : entity) {
        DeleteComponent(component.first, component.second);
    }

    auto destIndex = HandleToEntityIndex(handle);
    auto sourceIndex = m_entities.size() - 1;
    delete m_entities[destIndex];

    m_entities[destIndex] = m_entities[sourceIndex];
    m_entities.pop_back();

}

void ECS_Manager::UpdateSystems(ECSSystemList& systemList, float deltaTime)
{
    std::vector<BaseECSComponent*> componentParam;
    std::vector<std::vector<uint8_t>*> componentArrays;

    for(uint32_t i = 0; i < systemList.size(); i++)
    {
        auto componentTypes = systemList[i]->GetComponentTypes();
        if(componentTypes.size() == 1)
        {
            auto size = BaseECSComponent::GetTypeSize(componentTypes[0]);
            auto& data = m_components[componentTypes[0]];
            for(uint32_t j = 0; j < data.size(); j += size)
            {
                auto component = (BaseECSComponent*)&data[j];
                systemList[i]->UpdateComponents(deltaTime, &component);
            }
        }
        else
        {
            UpdateSystemWithMultiComponent(i, systemList, deltaTime, componentTypes, componentParam, componentArrays);
        }
    }
}

void ECS_Manager::DeleteComponent(uint32_t componentID, uint32_t index)
{
    std::vector<uint8_t>& data = m_components[componentID];

    auto freeFunc = BaseECSComponent::GetTypeFreeFunction(componentID);
    auto size = BaseECSComponent::GetTypeSize(componentID);
    uint32_t sourceIndex = data.size() - size;

    auto destComponent = (BaseECSComponent*)&data[index];
    auto sourceComponent = (BaseECSComponent*)&data[sourceIndex];

    freeFunc(destComponent);
    if(index == sourceIndex) {
        data.resize(sourceIndex);
        return;
    }

    std::memcpy(destComponent, sourceComponent, size);

    auto& entity = HandleToEntity(sourceComponent->m_entityID);
    for(auto& component : entity){
        if(componentID == component.first && sourceIndex == component.second) {
            component.second = index;
            break;
        }
    }

    data.resize(sourceIndex);
}

bool ECS_Manager::RemoveComponentInternal(EntityHandle handle, uint32_t componentID)
{
    auto& entity = HandleToEntity(handle);
    for (uint32_t i = 0; i < entity.size(); i++) {
        if (componentID == entity[i].first) {
            DeleteComponent(entity[i].first, entity[i].second);
            uint32_t sourceIndex = entity.size() - 1;
            uint32_t destIndex = i;
            entity[destIndex] = entity[sourceIndex];
            entity.pop_back();
            return true;
        }
    }
    return false;
}

void ECS_Manager::AddComponentInternal(EntityHandle handle, Entity& entity, uint32_t componentID, BaseECSComponent* component)
{
    auto createFunc = BaseECSComponent::GetTypeCreateFunction(componentID);
    entity.emplace_back(componentID, createFunc(m_components[componentID], handle, component));
}

BaseECSComponent* ECS_Manager::GetComponentInternal(Entity& entity, std::vector<uint8_t>& data,  uint32_t componentID)
{
    for (auto& component : entity) {
        if(componentID == component.first) {
            return (BaseECSComponent*)&data[component.second];
        }
    }
    return nullptr;
}

void ECS_Manager::UpdateSystemWithMultiComponent(uint32_t index, ECSSystemList& systemList, float deltaTime,
    const std::vector<uint32_t>& componentTypes, std::vector<BaseECSComponent*>& componentParam,
    std::vector<std::vector<uint8_t>*>& componentArrays)
{
    componentParam.resize(std::max(componentParam.size(), componentTypes.size()));
    componentArrays.resize(std::max(componentArrays.size(), componentTypes.size()));
    for(uint32_t i = 0; i < componentTypes.size(); i++) {
        componentArrays[i] = &m_components[componentTypes[i]];
    }

    auto minSizeIndex = FindLeastCommonComponent(componentTypes);

    auto size = BaseECSComponent::GetTypeSize(componentTypes[0]);
    std::vector<uint8_t>& data = *componentArrays[minSizeIndex];
    for (uint32_t i = 0; i < data.size(); i += size)
    {
        componentParam[0] = reinterpret_cast<BaseECSComponent*>(&data[i]);
        auto entity = HandleToEntity(componentParam[minSizeIndex]->m_entityID);

        bool isValid = true;
        for(uint32_t j = 0; j < componentTypes.size(); j++)
        {
            if (j == minSizeIndex) continue;

            componentParam[j] = GetComponentInternal(entity, *componentArrays[j], componentTypes[j]);
            if(componentParam[j] == nullptr) {
                isValid = false;
                break;
            }
        }

        if (isValid)
        {
            systemList[index]->UpdateComponents(deltaTime, &componentParam[0]);
        }
    }
}

uint32_t ECS_Manager::FindLeastCommonComponent(const std::vector<uint32_t>& componentTypes)
{
    uint32_t minSize = m_components[componentTypes[0]].size() / BaseECSComponent::GetTypeSize(componentTypes[0]);
    uint32_t minIndex = 0;
    for(uint32_t i = 1; i < componentTypes.size(); i++)
    {
        auto typesize = BaseECSComponent::GetTypeSize(componentTypes[i]);
        auto size = m_components[componentTypes[i]].size() / typesize;

        if(size < minSize) {
            minSize = size;
            minIndex = i;
        }
    }
    return minIndex;
}
