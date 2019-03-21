#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include "ECS_Component.h"
#include "ECS_System.h"
#include <map>

typedef std::vector<std::pair<uint32_t, uint32_t>> Entity;
typedef std::pair<uint32_t, Entity> EntityRaw;

class ATOM_API ECS_Manager
{
public:
    ECS_Manager() {};
    ~ECS_Manager();

#pragma region EntityMethods

    EntityHandle MakeEntity();
    EntityHandle MakeEntity(BaseECSComponent** components, const uint32_t* componentIDs, size_t numComponents);

    void RemoveEntity(EntityHandle handle);

#pragma endregion 

#pragma region ComponentMethods

    template<class T>
    inline void AddComponent(EntityHandle entity, T* component)
    {
        AddComponentInternal(entity, HandleToEntity(entity), T::ID, component);
    }

    template<class T>
    inline bool RemoveComponent(EntityHandle entity)
    {
        return RemoveComponentInternal(entity, T::ID);
    }

    template<class T>
    inline T* GetComponent(EntityHandle entity)
    {
        return (T*)GetComponentInternal(HandleToEntity(entity), m_components[T::ID], T::ID);
    }

#pragma endregion 

#pragma region SystemMethods

    void UpdateSystems(ECSSystemList& systemList, float deltaTime);
    
#pragma endregion 

private:
    std::vector<BaseECSSystem*> m_systems;
    std::map<uint32_t, std::vector<uint8_t>> m_components;
    std::vector<EntityRaw*> m_entities;

#pragma region EntityHandleMethods

    inline EntityRaw* HandleToRawType(EntityHandle handle)
    {
        return static_cast<EntityRaw*>(handle);
    }

    inline uint32_t HandleToEntityIndex(EntityHandle handle)
    {
        return HandleToRawType(handle)->first;
    }

    inline Entity& HandleToEntity(EntityHandle handle)
    {
        return HandleToRawType(handle)->second;
    }

#pragma endregion

#pragma region InternalComponentMethods

    void DeleteComponent(uint32_t componentID, uint32_t index);
    bool RemoveComponentInternal(EntityHandle handle, uint32_t componentID);
    void AddComponentInternal(EntityHandle handle, Entity& entity, uint32_t componentID, BaseECSComponent* component);
    BaseECSComponent* GetComponentInternal(Entity& entity, std::vector<uint8_t>& data, uint32_t componentID);

#pragma endregion

#pragma region InternalSystemMethods

    void UpdateSystemWithMultiComponent(uint32_t index, ECSSystemList& systemList, float deltaTime, const std::vector<uint32_t>& componentTypes,
                                        std::vector<BaseECSComponent*>& componentParam,
                                        std::vector<std::vector<uint8_t>*>& componentArrays);

    uint32_t FindLeastCommonComponent(const std::vector<uint32_t>& componentTypes);


#pragma endregion

};

