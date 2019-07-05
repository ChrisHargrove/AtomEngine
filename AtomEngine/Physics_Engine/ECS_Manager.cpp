#include "ECS_Manager.h"
#include "ThreadPool.h"


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

void ECS_Manager::ClearECS()
{
    for(int i = m_entities.size() - 1; i >= 0; --i)
    {
        Entity entity = m_entities[i]->second;
        for(auto& component : entity) {
            DeleteComponent(component.first, component.second);
        }
        delete m_entities[i];
        m_entities.pop_back();
    }
}

void ECS_Manager::UpdateSystems(ECSSystemList& systemList, float deltaTime)
{
    std::vector<BaseECSComponent*> componentParam;
    std::vector<std::vector<uint8_t>*> componentArrays;

    for(uint32_t i = 0; i < systemList.size(); i++)
    {
        auto componentTypes = systemList[i]->GetComponentTypes();

        bool validity = true;
        for (auto& type : componentTypes) {
            if (m_components[type].empty()) {
                validity = false;
                break;
            }
        }
        if (!validity) continue;

        if(componentTypes.size() == 1)
        {
            auto size = BaseECSComponent::GetTypeSize(componentTypes[0]);
            auto& data = m_components[componentTypes[0]];

            std::vector<BaseECSComponent*> components;
            components.reserve(data.size() / size);

            for(uint32_t j = 0; j < data.size(); j += size)
            {
                components.push_back((BaseECSComponent*)&data[j]);
            }
            systemList[i]->UpdateComponents(deltaTime, components);
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
    componentParam.resize((std::max)(componentParam.size(), componentTypes.size()));
    componentArrays.resize((std::max)(componentArrays.size(), componentTypes.size()));

    std::vector<std::vector<BaseECSComponent*>> components;
    std::vector<std::future< std::vector<BaseECSComponent*>>> jobs;

    for(uint32_t i = 0; i < componentTypes.size(); i++) {

        auto& compRef = m_components;

        jobs.push_back(JobSystem::Instance()->AddJob([i, &compRef, &componentArrays, &componentTypes]
        {
            componentArrays[i] = &compRef[componentTypes[i]];

            auto size = BaseECSComponent::GetTypeSize(componentTypes[i]);

            std::vector<BaseECSComponent*> comp;

            std::vector<uint8_t>& data = *componentArrays[i];

            for (uint32_t j = 0; j < data.size(); j += size)
            {
                comp.push_back((BaseECSComponent*)&data[j]);
            }

            return comp;

        }));
    }

    WaitForJobCompletion(jobs);

    for(auto& job : jobs) {
        components.push_back(job.get());
    }

    systemList[index]->UpdateComponents(deltaTime, components);
}
