#pragma once

#include "BroadPhase.h"
#include <set>

class BruteForce : public BroadPhase
{
public:
    BruteForce(){};
    virtual ~BruteForce(){};


    void Add(AABB* aabb) override {
        m_aabbList.insert(aabb);
    }

    void Remove(AABB* aabb) override {
        m_aabbList.erase(aabb);
    }

    void Clear() override {
        m_aabbList.clear();
    }

    void Update() override {
        //Not used in this broadphase
    }

    CollisionPairList CalculatePairs() override
    {
        m_collisionPairs.clear();
        for (auto i = m_aabbList.begin(); i != m_aabbList.end(); ++i) {
            auto jBegin = i;
            for(auto j = ++jBegin; j != m_aabbList.end(); ++j) {
                AABB* a = *i;
                AABB* b = *j;

                if(a->Collides(b)) {
                    m_collisionPairs.emplace_back(a, b);
                }
            }
        }
        return m_collisionPairs;
    }

private:
    std::set<AABB*> m_aabbList;
    CollisionPairList m_collisionPairs;
};
