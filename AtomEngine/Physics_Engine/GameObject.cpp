#include "GameObject.h"
#include "Component.h"
#include "Transform.h"

GameObject::GameObject() : 
m_name("GameObject"),
m_hasInitialized(false)
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
    std::unique_lock<std::mutex> lock(m_mutex);
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
    m_hasInitialized = true;
}

void GameObject::RemoveComponent(Component* comp)
{
    for (auto& it = m_componentList.begin(); it != m_componentList.end(); ) 
    {
        if ((*it)->GetTypeInfo() == comp->GetTypeInfo()) {
            it = m_componentList.erase(it);
        }
        else{
            ++it;
        }
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

void GameObject::AddChild()
{
    std::shared_ptr<GameObject> empty = std::make_shared<GameObject>();
    AddChild(empty);
}

std::vector<GameObject*> GameObject::GetChildren()
{
    std::vector<GameObject*> returnVal;
    for(auto& c_obj : m_childObjects)
    {
        returnVal.push_back(c_obj.get());
    }
    return returnVal;
}

std::vector<Component*> GameObject::GetComponents()
{
    std::vector<Component*> returnVal;
    for(auto& comp : m_componentList)
    {
        returnVal.push_back(comp.get());
    }
    return returnVal;
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

std::string& GameObject::GetName()
{
    return m_name;
}

void GameObject::SetName(const std::string& name)
{
    m_name = name;
}
