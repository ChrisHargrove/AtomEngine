#include "KDTree.h"
#include "Collider.h"
#include "BoxCollider.h"
#include "ShaderManager.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtx/norm.inl>

void CalculateBounds(const std::vector<RigidBody*>& objects, glm::vec3& minimum, glm::vec3& maximum)
{
    for (int i = 0; i < objects.size(); i++)
    {
        auto pos = objects[i]->m_transform->GetPosition();
        if (i == 0) {
            minimum = pos;
            maximum = pos;
        }
        else {
            if (minimum.x > pos.x) minimum.x = pos.x;
            if (minimum.y > pos.y) minimum.y = pos.y;
            if (minimum.z > pos.z) minimum.z = pos.z;

            if (maximum.x < pos.x) maximum.x = pos.x;
            if (maximum.y < pos.y) maximum.y = pos.y;
            if (maximum.z < pos.z) maximum.z = pos.z;
        }
    }
}

KDNode::KDNode(std::vector<RigidBody*> objects, int depth, int numDimensions):
    m_left(nullptr),
    m_right(nullptr)
{
    if (objects.empty()) { return; }

    //if depth has been reached then node contains rigid bodies
    if (depth == 0)
    {
        m_objects = std::vector<RigidBody*>(objects.begin(), objects.end());
        CalculateBounds(objects, m_minBounds, m_maxBounds);
        m_dimensions = glm::abs(m_maxBounds - m_minBounds);
        m_debugCube = new DebugCuboid(m_dimensions.x, m_dimensions.y, m_dimensions.z);
        return;
    }

    //calculate which axis we need to sort by.
    m_axis = depth % numDimensions;
    //calculate the median value in vector.
    auto median = objects.size() / 2;
    //if odd number of objects increment median.
    if (objects.size() % 2 == 1) {
        median++;
    }

    //Sort objects by the axis previously calculated
    switch (m_axis)
    {
    case 0: // X-Axis
        std::nth_element(objects.begin(), objects.begin() + median, objects.end(), CompareX());
        m_splitValue = objects[median]->m_transform->GetPosition().x;
        break;
    case 1: // Z-Axis
        std::nth_element(objects.begin(), objects.begin() + median, objects.end(), CompareZ());
        m_splitValue = objects[median]->m_transform->GetPosition().z;
        break;
    case 2: // Y-Axis
        std::nth_element(objects.begin(), objects.begin() + median, objects.end(), CompareY());
        m_splitValue = objects[median]->m_transform->GetPosition().y;
        break;
    }


    CalculateBounds(objects, m_minBounds, m_maxBounds);
    m_dimensions = glm::abs(m_maxBounds - m_minBounds);
    m_debugCube = new DebugCuboid(m_dimensions.x, m_dimensions.y, m_dimensions.z);

    //once sorted pass either side of median values to a new KDNode.
    m_left = new KDNode(std::vector<RigidBody*>(objects.begin(), objects.begin() + median), depth - 1, numDimensions);
    m_right = new KDNode(std::vector<RigidBody*>(objects.begin() + median, objects.end()), depth - 1, numDimensions);
}

KDNode::~KDNode()
{
    if(IsLeaf()) {
        delete m_debugCube;
        return;
    }

    delete m_left;
    delete m_right;
    delete m_debugCube;
    
}

void KDNode::Insert(RigidBody* object, int depth, int dimensions)
{
    if (IsLeaf())
    {
        m_objects.push_back(object);
        return;
    }
    auto pos = object->m_transform->GetPosition();

    switch (m_axis)
    {
    case 0: // X-Axis
        if (pos.x < m_splitValue) m_left->Insert(object, depth - 1, dimensions);
        else m_right->Insert(object, depth - 1, dimensions);
        break;
    case 1: // Z-Axis
        if (pos.z < m_splitValue) m_left->Insert(object, depth - 1, dimensions);
        else m_right->Insert(object, depth - 1, dimensions);
        break;
    case 2: // Y-Axis
        if (pos.y < m_splitValue) m_left->Insert(object, depth - 1, dimensions);
        else m_right->Insert(object, depth - 1, dimensions);
        break;
    }
}

void KDNode::Remove(RigidBody* object, int depth, int dimensions)
{
    if (IsLeaf())
    {
        m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
        return;
    }
    auto pos = object->m_transform->GetPosition();

    switch (m_axis)
    {
    case 0: // X-Axis
        if (pos.x < m_splitValue) m_left->Remove(object, depth - 1, dimensions);
        else m_right->Remove(object, depth - 1, dimensions);
        break;
    case 1: // Z-Axis
        if (pos.z < m_splitValue) m_left->Remove(object, depth - 1, dimensions);
        else m_right->Remove(object, depth - 1, dimensions);
        break;
    case 2: // Y-Axis
        if (pos.y < m_splitValue) m_left->Remove(object, depth - 1, dimensions);
        else m_right->Remove(object, depth - 1, dimensions);
        break;
    }
}

void KDNode::NearestNeighbour(RigidBody* rbody, RigidBody* neighbour, float& bestDistance)
{
    auto position = rbody->m_transform->GetPosition();
    if(IsLeaf())
    {
        //Find closest point out of current leaf
        float best = std::numeric_limits<float>::infinity();
        RigidBody* currentNeighbour = nullptr;

        for(auto body : m_objects) {
            auto calc = glm::length2(position - body->m_transform->GetPosition());
            if (calc < best) {
                best = calc;
                currentNeighbour = body;
            }
        }

        if(best < bestDistance) {
            bestDistance = best;
            neighbour = currentNeighbour;
        }

    }
    else
    {
        KDNode* searchFirst;
        float axisPosition;

        switch(m_axis) {
        case 0: // X-Axis
            axisPosition = position.x;
            break;
        case 1: // Z-Axis
            axisPosition = position.z;
            break;
        case 2: // Y-Axis
            axisPosition = position.y;
            break;
        }

        if(axisPosition <= m_splitValue) {
            searchFirst = m_left;
        }else {
            searchFirst = m_right;
        }

        if(searchFirst == m_left) {
            if (axisPosition - bestDistance <= m_splitValue) { m_left->NearestNeighbour(rbody, neighbour, bestDistance); }
            if (axisPosition + bestDistance > m_splitValue) { m_right->NearestNeighbour(rbody, neighbour, bestDistance); }
        }else {
            if (axisPosition + bestDistance > m_splitValue) { m_right->NearestNeighbour(rbody, neighbour, bestDistance); }
            if (axisPosition - bestDistance <= m_splitValue) { m_left->NearestNeighbour(rbody, neighbour, bestDistance); }
        }
    }
}


bool KDNode::IsLeaf() const
{
    return !m_left && !m_right;
}

void KDNode::DebugDraw()
{
    if (m_debugCube) {
        glm::mat4 model;
        model = glm::translate(model, m_minBounds + m_dimensions * 0.5f);
        Shaders::Instance()->GetShader("BASIC")->SetMat4("model", model);
        m_debugCube->Render();
    }

    if(m_left) m_left->DebugDraw();
    if(m_right) m_right->DebugDraw();
}

KDTree::KDTree(const std::vector<RigidBody*> objects, int maxDepth, int numDimensions) :
    m_count(0),
    m_objects(objects),
    m_maxDepth(maxDepth),
    m_numDimensions(numDimensions)
{
    m_root = new KDNode(objects, maxDepth, numDimensions);
}

KDTree::~KDTree()
{
    delete m_root;
}

void KDTree::Insert(RigidBody* object)
{
    m_objects.push_back(object);
    m_root->Insert(object, m_maxDepth, m_numDimensions);
    m_count++;
    if(m_count == 10) {
        Rebuild();
    }
}

void KDTree::Remove(RigidBody* object)
{
    m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
    m_root->Remove(object, m_maxDepth, m_numDimensions);
    m_count++;
    if (m_count == 10) {
        Rebuild();
    }
}

void KDTree::Rebuild()
{
    delete m_root;
    m_root = new KDNode(m_objects, m_maxDepth, m_numDimensions);
}

std::pair<RigidBody*, RigidBody*> KDTree::NearestNeighbour(RigidBody* rigidBody)
{
    RigidBody* neighbour = nullptr;
    float bestDistance = std::numeric_limits<float>::infinity();

    m_root->NearestNeighbour(rigidBody, neighbour, bestDistance);

    return std::make_pair(rigidBody, neighbour);
}

void KDTree::DebugDraw()
{
    m_root->DebugDraw();
}
