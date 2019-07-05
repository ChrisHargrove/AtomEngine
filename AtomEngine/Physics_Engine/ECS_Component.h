#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include <cstdint>
#include <vector>
#include <tuple>
#include <functional>

//Forward Declaration
struct BaseECSComponent;

/*!
 * \brief Wrapper for a void* to an Entity.
 */
typedef void* EntityHandle;

/*!
 * \brief Typedef Wrapper for the creation function of any given component type.
 * 
 * \param memory The block of memory that the components are stored in.
 * \param entity The handle to the entity that the component belongs to.
 * \param component A pointer to the component that you are creating.
 */
typedef std::function<uint32_t(std::vector<uint8_t>& memory, EntityHandle entity, BaseECSComponent* component)> ECSComponentCreateFunction;

/*!
 * \brief Typedef Wrapper for the deletion function of any given component type.
 * 
 * \param component A pointer to the component you wish to free the memory of.
 */
typedef std::function<void(BaseECSComponent* component)> ECSComponentFreeFunction;

/*!
 * \brief Typedef Wrapper for the component type data.
 */
typedef std::tuple<ECSComponentCreateFunction, ECSComponentFreeFunction, size_t> ComponentType;

/*!
 * \brief The base class of all components.
 */
struct ATOM_API BaseECSComponent
{
public:

    /*!
     * \brief Will Register the new component alongside its creation and deletion functions as well as its size.
     * 
     * \param createFunc The creation function for the component.
     * \param freeFunc The deletion function for the component.
     * \param size The size of the component in bytes.
     * \return The ID of the new component.
     */
    static uint32_t RegisterComponentType(ECSComponentCreateFunction createFunc, ECSComponentFreeFunction freeFunc, size_t size);

    /*!
     * \brief The entity ID as a handle, which is just a wrapper for a void*.
     */
    EntityHandle m_entityID = nullptr;

    /*!
     * \brief Gets the components Creation Function.
     * \param id The type ID of the component.
     * \return A std::function containing the creation function.
     */
    inline static ECSComponentCreateFunction GetTypeCreateFunction(uint32_t id)
    {
        return std::get<0>((*m_componentTypes)[id]);
    }

    /*!
     * \brief Gets the components Deletion Function.
     * \param id The type ID of the component.
     * \return A std::function containing the deletion function.
     */
    inline static ECSComponentFreeFunction GetTypeFreeFunction(uint32_t id)
    {
        return std::get<1>((*m_componentTypes)[id]);
    }

    /*!
     * \brief Gets the components size.
     * \param id The type ID of the component.
     * \return The size of the component in Bytes.
     */
    inline static size_t GetTypeSize(uint32_t id)
    {
        return std::get<2>((*m_componentTypes)[id]);
    }

    /*!
     * \brief Checks to see if the Component ID is valid.
     * \param id The ID of the component type.
     * \return Bool representing if the component type is valid or not.
     */
    inline static bool IsValidType(uint32_t id)
    {
        return id < m_componentTypes->size();
    }

private:
    /*!
     * \brief A Static container of all component types.
     */
    static std::vector<ComponentType>* m_componentTypes;
};

/*!
 * \brief The actual inherited class for all components.
 * 
 * Usage is as such: MyComponent : public ECSComponent<MyComponent>
 */
template<typename T>
struct ECSComponent : public BaseECSComponent
{
    static const ECSComponentCreateFunction CREATE_FUNC;
    static const ECSComponentFreeFunction FREE_FUNC;

    static const uint32_t ID;
    static const size_t SIZE;
};

/*!
 * \brief The template creation function for any given type.
 * \param memory The memory location where the new component is to be created.
 * \param entity The entity handle that the component is assigned to.
 * \param component The component type that is going to be created.
 * \return The component ID.
 */
template<typename T>
uint32_t ECSComponentCreate(std::vector<uint8_t>& memory, EntityHandle entity, BaseECSComponent* component)
{
    //The last position in the vector is the next index.
    auto index = memory.size();
    //Resize the vector to have enough room for the new component.
    memory.resize(index + T::SIZE);
    //Use placement new operator to create a new component that is a copy of the other specified, in the memory index.
    T* comp = new(&memory[index])T(*static_cast<T*>(component));
    //Set the new component to have a copy of its owners handle.
    comp->m_entityID = entity;
    //Return the index of the newly created component.
    return index;
}

/*!
 * \brief The template free function for any given type.
 * \param component A pointer to the component to free.
 */
template<typename T>
void ECSComponentFree(BaseECSComponent* component)
{
    //Cast the component to the required type.
    T* comp = static_cast<T*>(component);
    //Manually call the types destructor.
    comp->~T();
}

template<typename T>
const uint32_t ECSComponent<T>::ID(BaseECSComponent::RegisterComponentType(ECSComponentCreate<T>, ECSComponentFree<T>, sizeof(T)));

template<typename T>
const size_t ECSComponent<T>::SIZE(sizeof(T));

template<typename T>
const ECSComponentCreateFunction ECSComponent<T>::CREATE_FUNC(ECSComponentCreate<T>);

template<typename T>
const ECSComponentFreeFunction ECSComponent<T>::FREE_FUNC(ECSComponentFree<T>);
