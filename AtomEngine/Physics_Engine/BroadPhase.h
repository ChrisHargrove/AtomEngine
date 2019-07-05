#pragma once
#include "AABB.h"
#include "DebugRenderer.h"
#include <list>

typedef std::pair<AABB*, AABB*> CollisionPair;
typedef std::list<CollisionPair> CollisionPairList;

class BroadPhase
{
public:
    BroadPhase(DebugRenderer* debugRenderer){}
    virtual ~BroadPhase(){}

    virtual void Add(AABB* aabb) = 0;
    virtual void Remove(AABB* aabb) = 0;
    virtual void Clear() = 0;
    virtual void Update() = 0;

    virtual bool* GetShowDebug() = 0;
    virtual int GetChecksMade() { return 0; };

    virtual CollisionPairList CalculatePairs() = 0;

protected:
    DebugCuboid m_debugCuboid;
    DebugRenderer* m_debugRenderer;
};
