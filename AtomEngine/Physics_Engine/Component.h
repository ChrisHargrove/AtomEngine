/*!
    * \class Component "Component.h"
    * \brief Component class for use in the Entity Component System
    *
    * This is the base class for the Entity Component system (ECS), all objects
    * used inside the game engine are a component, this way game objects are just
    * a container of components, each component acts as either a piece of the object
    * or a form of behaviour.
*/
#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include "GameObject.h"

#include <memory>
#include <CEREAL/cereal.hpp>
#include <CEREAL/types/memory.hpp>
#include <CEREAL/archives/xml.hpp>

class ATOM_API Component
{
public:
    Component() {
        m_name = "";
    };
    virtual ~Component() {};

    /*!
        * \brief Virtual Update Function.
        * \param deltaTime The time passed since the last frame.
        *
        * Will call the appropriate function for each component by polymorphism.
    */
    virtual void Update(float deltaTime) = 0;

    /*!
        * \brief Virtual Initialize Function.
        *
        * Will call the appropriate fucntion for each component by polymorphism.
    */
    virtual void Initialize() = 0;

    /*!
        * \brief Sets the owning parent object.
        * \param parent The parent GameObject that owns this component.
        *
        * Will set the parent object for this component, this is so that each compenent
        * can retrieve data from the parent and allows for inter component communication.
    */
    void SetParent(std::shared_ptr<GameObject> parent) { m_parent = parent; }

    /*!
        * \brief Gets the owning GameObject for this component.
        * \return Returns a the owning GameObject pointer for this component.
    */
    GameObject* GetParent() 
    { 
        if (m_parent.lock() != NULL) return m_parent.lock().get();
        else { return nullptr; }
    }

    std::string GetName() {
        return m_name;
    }

    /*!
        * \brief Gets a Component from the parent GameObject.
        * \return Returns a pointer to the found Component or nullptr if there is no matching component.
        *
        * Will check the parent for a Component of specified type and return it.
    */
    template<class T>
    T* GetComponent();

    template <class Archive>
    void serialize(Archive& archive) {};

protected:
    std::string m_name;
    std::weak_ptr<GameObject> m_parent; /*!< A pointer to the owning parent game object. */
};

template<class T>
inline T* Component::GetComponent()
{
    if(m_parent.lock() != NULL) return m_parent.lock().get()->GetComponent<T>();
    else { return nullptr; }
}

extern template ATOM_API void Component::serialize<cereal::XMLInputArchive>
(cereal::XMLInputArchive & archive);

extern template ATOM_API void Component::serialize<cereal::XMLOutputArchive>
(cereal::XMLOutputArchive & archive);