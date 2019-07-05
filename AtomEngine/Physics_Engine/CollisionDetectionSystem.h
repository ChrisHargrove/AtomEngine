#pragma once
#include "ECS_System.h"
#include "AABBComponent.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "BroadPhase.h"
#include "LogManager.h"
#include <set>
#include "NarrowPhase.h"

class CollisionDetectionSystem : public BaseECSSystem
{
public:
    CollisionDetectionSystem() {
        AddComponentType(TransformComponent::ID);
        AddComponentType(AABBComponent::ID);
        AddComponentType(MeshComponent::ID);
    }

    ~CollisionDetectionSystem() override {
        delete m_broadPhase;
    }

    template<class T>
    void SetBroadPhase(DebugRenderer* debugRenderer) {
        m_broadPhase = new T(debugRenderer);
    }

    template<class T>
    void SetNarrowPhase() {
        m_narrowPhase = new T();
    }

    virtual void UpdateComponents(float deltaTime, std::vector<std::vector<BaseECSComponent*>>& componentArrays) override
    {

        //collate all the aabb's into a set
        std::set<AABB*> newList;
        for (uint32_t i = 0; i < componentArrays[0].size(); i++)
        {
            POD_Transform* transform = &((TransformComponent*)componentArrays[0][i])->m_transform;
            AABB* aabb = &((AABBComponent*)componentArrays[1][i])->m_aabb;
            POD_Mesh* mesh = &((MeshComponent*)componentArrays[2][i])->m_mesh;

            aabb->IsColliding() = AABB::NO_COLLISION;
            aabb->RecalculateAABB(transform, mesh);
            newList.insert(aabb);
            
        }

        //find which aabb's are no longer in existence
        //and any new ones that need to be added
        HandleNewAndOld(newList);

        //m_broadPhase->SetShowDebug(true);
        Profiler::Instance()->Start("Update BroadPhase");
        m_broadPhase->Update();
        Profiler::Instance()->End("Update BroadPhase");

        Profiler::Instance()->Start("BroadPhase Collision Detection");
        auto collisions = m_broadPhase->CalculatePairs();
        Profiler::Instance()->End("BroadPhase Collision Detection");

        Logger::Instance()->LogInfo("BruteForce Checks: " + std::to_string(m_aabbList.size() * m_aabbList.size()));
        Logger::Instance()->LogInfo("Actual Checks Made: " + std::to_string(m_broadPhase->GetChecksMade()));
        Logger::Instance()->LogInfo("Potential Collisions Found: " + std::to_string(collisions.size()));

        Profiler::Instance()->Start("NarrowPhase Collision Detection");
        collisions = m_narrowPhase->GetCollisions(collisions);
        Profiler::Instance()->End("NarrowPhase Collision Detection");
    }

    bool* GetShowBroadphaseDebug() {
        return m_broadPhase->GetShowDebug();
    }

private:
    BroadPhase* m_broadPhase{};
    NarrowPhase* m_narrowPhase{};

    std::set<AABB*> m_aabbList;

    void HandleNewAndOld(std::set<AABB*>& newList) {
        std::set<AABB*> removeList;
        auto removeIT = std::set_difference(m_aabbList.begin(), m_aabbList.end(), newList.begin(), newList.end(), std::inserter(removeList, removeList.end()));

        std::set<AABB*> addList;
        auto addIT = std::set_difference(newList.begin(), newList.end(), m_aabbList.begin(), m_aabbList.end(), std::inserter(addList, addList.end()));

        for (AABB* aabb : removeList) {
            m_broadPhase->Remove(aabb);
            m_aabbList.erase(aabb);
        }

        for (AABB* aabb : addList) {
            m_broadPhase->Add(aabb);
            m_aabbList.insert(aabb);
        }
    }
};
