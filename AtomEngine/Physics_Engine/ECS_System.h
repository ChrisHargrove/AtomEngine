#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include "ECS_Component.h"
#include <vector>

/*!
 * \brief The Base class for all systems in the ECS.
 */
class ATOM_API BaseECSSystem
{
public:
    /*!
     * \brief Default Constructor
     */
    BaseECSSystem() = default;

    /*!
     * \brief Default Destructor
     */
    virtual ~BaseECSSystem() = default;

    /*!
     * \brief Will update all the component associated with this system.
     * \param deltaTime The time passed since the last update.
     * \param components Double pointer to the container of components.
     */
    virtual void UpdateComponents(float deltaTime, BaseECSComponent** components){}

    virtual void UpdateComponents(float deltaTime, std::vector<BaseECSComponent*>& components){}

    virtual void UpdateComponents(float deltaTime, std::vector<std::vector<BaseECSComponent*>>& componentArrays){}

    /*!
     * \brief Gets the component types associated with this system.
     * \return The component types required by this system to operate.
     */
    inline const std::vector<uint32_t>& GetComponentTypes() {
        return m_componenTypes;
    }

protected:
    /*!
     * \brief Adds a new component type to this system.
     * \param componentID The component type ID.
     * 
     * This will add a new component type to the system that when updating the system will be retrieved.
     */
    inline void AddComponentType(uint32_t componentID) {
        m_componenTypes.push_back(componentID);
    }

private:
    /*!
     * \brief A container of required component types for this system.
     */
    std::vector<uint32_t> m_componenTypes;
};

class ATOM_API ECSSystemList
{
public:
    ECSSystemList() {};
    ~ECSSystemList() {};

    inline void AddSystem(BaseECSSystem* system) {
        m_systems.push_back(system);
    }

    inline size_t size() const {
        return m_systems.size();
    }

    inline BaseECSSystem* operator[](uint32_t index) {
        return m_systems[index];
    }

    bool RemoveSystem(BaseECSSystem& system);

private:
    std::vector<BaseECSSystem*> m_systems;
};
