#include "GameObject.h"
#include "Component.h"
#include "Transform.h"

GameObject::GameObject()
{
    AddComponent<Transform>();
    Initialize();
}


GameObject::~GameObject()
{
    for (auto child : m_childObjects) {
        delete child;
    }
    for (auto component : m_componentList) {
        delete component;
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



