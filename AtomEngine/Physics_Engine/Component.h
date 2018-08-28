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

class GameObject;

class ATOM_API Component
{
public:
    Component() {};
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
    void SetParent(GameObject* parent) { m_parent = parent; }

    //TODO: Need to add get component functions for component class that are virtual and can be overriden. This will allow components to talk to ecah other inside 1 game object...needs refining....
    //DATE: 12/07/2018....

protected:
    GameObject * m_parent; /*!< A pointer to the owning parent game object. */
};

