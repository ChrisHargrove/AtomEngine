/*!
    * \class GameObject "GameObject.h"
    * \brief Container class for every object in the engine.
    * 
    * This class is the base for all objects inside the game. The game object is also a component meaning
    * meaning that GameObject's can be a collection of component and other game objects.
*/
#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include <list>
#include <vector>
#include <typeinfo>
#include <memory>

#include <CEREAL/cereal.hpp>

class Component;

class ATOM_API GameObject : public std::enable_shared_from_this<GameObject>
{
public:
    GameObject();
    ~GameObject();

    /*!
        * \brief Virtual Update Function that is called once per frame.
        * \param deltaTime The time passed since the last frame.
        *
        * This is where all child components and child objects get updated. Which in turn calls each
        * of their update functions through polymorphism.
    */
    virtual void Update(float deltaTime);

    /*!
        * \brief Virtual Initialize Function that is called only once.
        *
        * This is where all child components and child objects get initialized. This in turn calls
        * each of their Initialize functions.
    */
    virtual void Initialize();

    /*!
        * \brief Adds a component to the GameObject.
        * 
        * This is a templated function that allows for any type of component to be added. This function
        * will then create a new instance of the component type and add it to the game object.
    */
    template<class T>
    void AddComponent();

    /*!
    * \brief Adds a component to the GameObject.
    * \param component The component to be added to the Game Object.
    * This is a templated function that allows for any type of component to be added. This function
    * will then create a new instance of the component type and add it to the game object.
    */
    template<class T>
    void AddComponent(std::shared_ptr<T> component);

    /*!
        * \brief Removes a component from the GameObject.
        *
        * This is a templated function that will remove any type of component from the game object. This function
        * will search through the component list until it finds a matching component type and will remove the first one found.
        * If no matching type is found then the function does nothing.
    */
    template<class T>
    void RemoveComponent();

    /*!
        * \brief Removes all components of type specified from the GameObject.
        *
        * This is a templated function that will remove all of any type of component from the game object. This function
        * will search through the component list until it finds a matching component type and will remove it and continue looking for others.
        * If no matching type is found then the function does nothing.
    */
    template<class T>
    void RemoveAllComponents();

    /*!
        * \brief Gets a component from the GameObject.
        * \return Returns a pointer to the component found, if component type not found returns nulltpr.
        *
        * This is a templated function that allows for the retrieval of any component type from inside the GameObject.
        * If no matching component type is found then function returns a nullptr.
    */
    template<class T>
    T* GetComponent();

    /*!
        * \brief Gets a component from the parent object.
        * \return Returns a pointer to the component found, if component type not found returns nullptr.
        *
        * This is a templated function that allows for the retrieval of any component type from the parent object.
        * If no matching component type is found then returns a nullptr.
    */
    template<class T>
    T* GetComponentInParent();

    /*!
        * \brief Gets the first component found of type specified in child objects.
        * \return Returns a pointer to the component found. If no components were found returns nullptr.
        *
        * This is a templated function that allows for the retrieval of any component type from child objects of a GameObject.
        * If no matching component type is found then returns a nullptr.
    */
    template<class T>
    T* GetComponentInChild();

    /*!
        * \brief Gets a list of all components of type specified from all child objects in GameObject.
        * \return Returns a list by value, of components found in all children of GameObject.
        *
        * This is a templated function that allows for the retrieval of any component type from the child objects of a GameObject,
        * it will however return a pointer to all instance of components of that type.
    */
    template<class T>
    std::vector<T*> GetComponentsInChildren();

    /*!
        * \brief Gets a pointer to parent GameObject
        * \return Returns a pointer to the parent GameObject.
    */
    GameObject* GetParent();

    /*!
        * \brief Sets the parent of this GameObject
        * \param parent The GameObject you wish to become this GameObjects new parent.
    */
    void SetParent(std::shared_ptr<GameObject> parent);

    /*!
        * \brief Adds a child GameObject to this GameObject.
        * \param child The GameObject to add.
        *
        * Will add the supplied GameObject to the current GameObject and set it as the 
        * child objects parent.
    */
    void AddChild(std::shared_ptr<GameObject> child);

    //TODO: Add Remove Child function, this will require some sort of ID system...maybe GUID?

    /*!
        * \brief Destroys a GameObject.
        * \param obj A reference to the obj you wish to destroy.
        *
        * Will travel through the child list and their corresponding component lists until all
        * smart pointers are released. After this will then release the GameObjects component list
        * smart pointers and will finally release the GameObject itself. Calling the automatic destructor
        * for the game object.
    */
    static void Destroy(std::shared_ptr<GameObject> &obj);

protected:
    std::vector<std::shared_ptr<Component>> m_componentList;  /*!< The list of components inside the GameObject. */
    std::weak_ptr<GameObject> m_parentObject; /*!< The parent of this GameObject. */
    std::vector<std::shared_ptr<GameObject>> m_childObjects; /*!< The list of child objects attached to the GameObject. */

private:

    bool m_hasInitialized; /*!< Whether the game object has already been initialized or not. */

    friend class cereal::access; // Allows for private serialization methods //

    /*!
        * \brief Serializes the GameObject to a file for later use.
        * \param archive A reference to the Cereal Archive being used.
        *
        * This function never gets called by the user hence why it is a private method. This will take each
        * part of the GameObject and serialize it to a file previously specified with the IOManager.
        * Saves each Component with its type as a name.
    */
    template<class Archive>
    void save(Archive &archive) const {
        for (int i = 0; i < m_componentList.size(); i++) {
            archive(cereal::make_nvp(m_componentList[i]->GetName(), m_componentList[i]));
        }
    }

    /*!   
        * \brief Serializes the GameObject from a file.
        * \param archive A reference to the Cereal Archive being used.
        *
        * This function never gets called by the user hence why it is a private method. This will take an archive
        * object and load all the data from it into the GameObject.
    */
    template<class Archive>
    void load(Archive &archive) {
        int nodeSize;
        archive.loadSize(nodeSize);
        for (int i = 0; i < nodeSize; i++) {
            std::shared_ptr<Component> newComponent;
            archive(newComponent);
            AddComponent(newComponent);
        }
    }
};

template<class T>
inline void GameObject::AddComponent()
{
    std::shared_ptr<T> component = std::make_shared<T>();
    component->SetParent(shared_from_this());
    m_componentList.push_back(component);

    if(m_hasInitialized)
    {
        component->Initialize();
    }
}

template<class T>
inline void GameObject::AddComponent(std::shared_ptr<T> component)
{
    component.get()->SetParent(shared_from_this());
    m_componentList.push_back(component);
}

template<class T>
inline void GameObject::RemoveComponent() 
{
    for (auto component : m_componentList) {
        if (typeid(*component.get()) == typeid(T)) {
            m_componentList.erase(component);
            break;
        }
    }
}

template<class T>
inline void GameObject::RemoveAllComponents()
{
    for (auto component : m_componentList) {
        if (typeid(*component.get()) == typeid(T)) {
            m_componentList.erase(component);
        }
    }
}

template<class T>
inline T* GameObject::GetComponent()
{
    for (auto component : m_componentList) {
        if (typeid(*component.get()) == typeid(T)) {
            return dynamic_cast<T*>(component.get());
        }
    }
    return nullptr;
}

template<class T>
inline T* GameObject::GetComponentInParent()
{
    for (auto component : m_parentObject.get()->m_componentList) {
        if (typeid(*component.get()) == typeid(T)) {
            return component.get();
        }
    }
    return nullptr;
}

template<class T>
inline T* GameObject::GetComponentInChild() 
{
    for (auto child : m_childObjects) {
        for (auto component : child.get()->m_componentList) {
            if (typeid(*component.get()) == typeid(T)) {
                return component.get();
            }
        }
    }
    return nullptr;
}

template<class T>
inline std::vector<T*> GameObject::GetComponentsInChildren()
{
    std::vector<T*> componentList;
    for (auto child : m_childObjects) {
        for (auto component : child.get()->m_componentList) {
            componentList.push_back(component.get());
        }
    }
    return componentList;
}
