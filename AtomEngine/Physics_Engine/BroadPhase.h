#pragma once

#include <vector>
#include "RigidBody.h"

class BroadPhase
{
public:
    BroadPhase() = default;
    virtual ~BroadPhase() = default;

    virtual void UpdateAABBList(const std::vector<RigidBody*>& bodiesList) = 0;
    virtual const std::vector<std::pair<int, int> >& GenerateOverlapList() = 0;
};

