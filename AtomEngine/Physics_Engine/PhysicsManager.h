#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif
#include "AlignedAllocation.h"
#include "Singleton.h"
#include "RigidBody.h"
#include "KDTree.h"


class ATOM_API PhysicsManager : public AlignedAllocation<BYTE16>
{
public:
    friend class Singleton<PhysicsManager>;

    void Initialize();
    void Shutdown();
    void Reset();

    void StepSimulation(float deltaTime);
    void DrawDebug();

    void AddBody(RigidBody* body);
    void RemoveBody(RigidBody* body);

    bool& StartSimulation();
    bool& ShouldDrawDebug();
    bool& ShouldDrawColliders();
    bool& ShouldDrawKDTree();

private:
    PhysicsManager() {};
    PhysicsManager(const PhysicsManager&);

    std::vector<RigidBody*> m_bodies;

    KDTree* m_kdTree;

    bool m_stepSimulation;
    bool m_drawDebug;
    bool m_drawColliders;
    bool m_drawKDTree;

    float m_gravityMultiplier;
    float m_gravityAcceleration;
};

template ATOM_API PhysicsManager* Singleton<PhysicsManager>::Instance();

typedef Singleton<PhysicsManager> Physics;
