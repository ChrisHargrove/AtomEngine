#pragma once

#include "RigidBody.h"
#include "Transform.h"
#include "PhysicsTypes.h"
#include <algorithm>
#include "DebugCuboid.h"

struct CompareX {
    bool operator()(const RigidBody* left, const RigidBody* right) {
        auto l = left->GetComponent<Transform>()->GetPosition();
        auto r = right->GetComponent<Transform>()->GetPosition();
        return l.x < r.x;
    }
};

struct CompareY {
    bool operator()(const RigidBody* left, const RigidBody* right) {
        auto l = left->GetComponent<Transform>()->GetPosition();
        auto r = right->GetComponent<Transform>()->GetPosition();
        return l.y < r.y;
    }
};

struct CompareZ {
    bool operator()(const RigidBody* left, const RigidBody* right) {
        auto l = left->GetComponent<Transform>()->GetPosition();
        auto r = right->GetComponent<Transform>()->GetPosition();
        return l.z < r.z;
    }
};

void CalculateBounds(const std::vector<RigidBody*>& objects, glm::vec3& minimum, glm::vec3& maximum);

class KDNode
{
public:
    KDNode(std::vector<RigidBody*> objects, int depth, int numDimensions);

    ~KDNode();

    void Insert(RigidBody* object, int depth, int dimensions);

    void Remove(RigidBody* object, int depth, int dimensions);

    bool IsLeaf() const;

    void DebugDraw();

private:
    glm::vec3 m_median;
    KDNode* m_left;
    KDNode* m_right;

    std::vector<RigidBody*> m_objects;

    //Debug Drawing
    glm::vec3 m_minBounds;
    glm::vec3 m_maxBounds;
    glm::vec3 m_dimensions;
    DebugCuboid* m_debugCube;
};

class KDTree
{
public:
    KDTree(const std::vector<RigidBody*> objects, int maxDepth, int numDimensions);
    ~KDTree();

    void Insert(RigidBody* object);
    void Remove(RigidBody* object);

    void Rebuild();

protected:
    friend class PhysicsManager;

    void DebugDraw();

private:
    KDNode* m_root;
    int m_maxDepth;
    int m_numDimensions;

    int m_count;
    std::vector<RigidBody*> m_objects;
};
