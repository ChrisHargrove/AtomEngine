#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include "AlignedAllocation.h"
#include "Singleton.h"
#include "LogManager.h"
#include <typeindex>

#include <map>
#include <any>
#include <string>

/*  TODO:: Add all the destruction stuff so what happens when the program closes down...
    Might be bale to make it count the ref number of shared_ptr before destruction to check
    if all objects have been released correctly which will help with memory leaks!! */

template<class T>
class __declspec(dllexport) ResourceHolder : public AlignedAllocation<BYTE16>
{
public:
    ResourceHolder();
    ~ResourceHolder();

    bool AddResource(std::string name, std::shared_ptr<T> resource);
    bool RemoveResource(std::string name);

    bool HasResource(std::string name);
    std::shared_ptr<T> GetResource(std::string name);

private:
    std::map<std::string, std::shared_ptr<T>> m_resourceList;
};

template <class T>
ResourceHolder<T>::ResourceHolder()
{
}

template <class T>
ResourceHolder<T>::~ResourceHolder()
{
}

template <class T>
bool ResourceHolder<T>::AddResource(std::string name, std::shared_ptr<T> resource)
{
    auto search = m_resourceList.find(name);
    if(search == m_resourceList.end()) {
        m_resourceList.emplace(std::make_pair(name, resource));
        return true;
    }
    Logger::Instance()->LogWarning("[RESOURCES] Resource: " + name + " already exists...Skipping...");
    return false;
}

template <class T>
bool ResourceHolder<T>::RemoveResource(std::string name)
{
    auto search = m_resourceList.find(name);
    if(search != m_resourceList.end()) {
        m_resourceList.erase(name);
        return true;
    }
    Logger::Instance()->LogWarning("[RESOURCES] Resource: " + name + " doesn't exist...Skipping...");
    return false;
}

template <class T>
bool ResourceHolder<T>::HasResource(std::string name)
{
    auto search = m_resourceList.find(name);
    if (search != m_resourceList.end())return true;
    Logger::Instance()->LogWarning("[RESOURCES] Resource Holder does not contain " + name);
    return false;
}

template <class T>
std::shared_ptr<T> ResourceHolder<T>::GetResource(std::string name)
{
    return m_resourceList.at(name);
}


class ATOM_API ResourceManager : public AlignedAllocation<BYTE16>
{
public:
    friend class Singleton<ResourceManager>;

    template <class T>
    bool AddResource(std::string name, std::shared_ptr<T> resource);

    template <class T>
    std::shared_ptr<T> GetResource(std::string name);

    template <class T>
    bool HasResource(std::string name);

private:
    ResourceManager() {};
    ResourceManager(const ResourceManager&) {};

    std::map<std::type_index, std::any> m_resourceHolders;
};

template <class T>
bool ResourceManager::AddResource(std::string name, std::shared_ptr<T> resource)
{
    auto search = m_resourceHolders.find(typeid(T));
    if(search != m_resourceHolders.end()) {
        auto potential = m_resourceHolders.at(typeid(T));
        auto holder = std::any_cast<ResourceHolder<T>>(potential);
        return holder.AddResource(name, resource);
    }
    else {
        ResourceHolder<T> newHolder;
        if(newHolder.AddResource(name, resource)) {
            std::type_index type = typeid(T);
            m_resourceHolders.emplace(std::make_pair(type, newHolder));
        }
        return true;
    }
}

template <class T>
std::shared_ptr<T> ResourceManager::GetResource(std::string name)
{
    auto search = m_resourceHolders.find(typeid(T));
    if(search != m_resourceHolders.end()) {
        std::any potential = m_resourceHolders.at(typeid(T));
        ResourceHolder<T> holder = std::any_cast<ResourceHolder<T>>(potential);
        if(holder.HasResource(name)) {
            return holder.GetResource(name);
        }
    }
    else {
        Logger::Instance()->LogWarning("[RESOURCES] Manager doesn't contain holder for typeid of " + name);
        return nullptr;
    }
    return nullptr;
}

template <class T>
bool ResourceManager::HasResource(std::string name)
{
    auto search = m_resourceHolders.find(typeid(T));
    if(search != m_resourceHolders.end()) {
        std::any potential = m_resourceHolders.at(typeid(T));
        ResourceHolder<T> holder = std::any_cast<ResourceHolder<T>>(potential);
        return holder.HasResource(name);
    }else {
        return false;
    }
}

template ATOM_API ResourceManager* Singleton<ResourceManager>::Instance();

typedef Singleton<ResourceManager> Resource;

