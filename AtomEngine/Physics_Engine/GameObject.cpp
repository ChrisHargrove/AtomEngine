#include "GameObject.h"
#include "Component.h"
#include "Transform.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
    for (auto child : m_childObjects) {
        child.reset();
    }
    for (auto component : m_componentList) {
        component.reset();
    }
    m_componentList.clear();
    
}

void GameObject::Update(float deltaTime)
{
    for (auto component : m_componentList) {
        component->Update(deltaTime);
    }
    for (auto child : m_childObjects) {
        child->Update(deltaTime);
    }
}

void GameObject::Initialize()
{
    for(auto component : m_componentList) {
        component->Initialize();
    }
    for (auto child : m_childObjects) {
        child->Initialize();
    }
}

GameObject * GameObject::GetParent()
{
    if (m_parentObject.lock() != NULL) {
        return m_parentObject.lock().get();
    }
    else {
        return nullptr;
    }
}

void GameObject::SetParent(std::shared_ptr<GameObject> parent)
{
    m_parentObject = parent;
}

void GameObject::AddChild(std::shared_ptr<GameObject> child)
{
    child->SetParent(shared_from_this());
    m_childObjects.push_back(child);
}

void GameObject::Destroy(std::shared_ptr<GameObject> &obj)
{
    for (int i = 0; i < obj->m_childObjects.size(); i++) {
        for (int j = 0; j < obj->m_childObjects[i].get()->m_componentList.size(); j++) {
            obj->m_childObjects[i].get()->m_componentList[j].reset();
        }
        obj->m_childObjects[i].reset();
    }
    for (int i = 0; i < obj->m_componentList.size(); i++) {
        obj->m_componentList[i].reset();
    }
    obj.reset();
}
