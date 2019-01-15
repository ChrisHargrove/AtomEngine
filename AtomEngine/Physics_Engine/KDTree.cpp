#include "KDTree.h"
#include "Collider.h"
#include "BoxCollider.h"
#include "ShaderManager.h"
#include <GLM/gtc/matrix_transform.hpp>

void CalculateBounds(const std::vector<RigidBody*>& objects, glm::vec3& minimum, glm::vec3& maximum)
{
    for (int i = 0; i < objects.size(); i++)
    {
        auto pos = objects[i]->GetComponent<Transform>()->GetPosition();
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
    auto axis = depth % numDimensions;
    //calculate the median value in vector.
    auto median = objects.size() / 2;
    //if odd number of objects increment median.
    if (objects.size() % 2 == 1) {
        median++;
    }

    //Sort objects by the axis previously calculated
    switch (axis)
    {
    case 0: // X-Axis
        std::nth_element(objects.begin(), objects.begin() + median, objects.end(), CompareX());
        break;
    case 1: // Z-Axis
        std::nth_element(objects.begin(), objects.begin() + median, objects.end(), CompareZ());
        break;
    case 2: // Y-Axis
        std::nth_element(objects.begin(), objects.begin() + median, objects.end(), CompareY());
        break;
    }

    m_median = objects[median]->GetComponent<Transform>()->GetPosition();

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
    auto axis = depth % dimensions;
    auto pos = object->GetComponent<Transform>()->GetPosition();

    switch (axis)
    {
    case 0: // X-Axis
        if (pos.x < m_median.x) m_left->Insert(object, depth - 1, dimensions);
        else m_right->Insert(object, depth - 1, dimensions);
        break;
    case 1: // Z-Axis
        if (pos.z < m_median.z) m_left->Insert(object, depth - 1, dimensions);
        else m_right->Insert(object, depth - 1, dimensions);
        break;
    case 2: // Y-Axis
        if (pos.y < m_median.y) m_left->Insert(object, depth - 1, dimensions);
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
    auto axis = depth % dimensions;
    auto pos = object->GetComponent<Transform>()->GetPosition();

    switch (axis)
    {
    case 0: // X-Axis
        if (pos.x < m_median.x) m_left->Insert(object, depth - 1, dimensions);
        else m_right->Remove(object, depth - 1, dimensions);
        break;
    case 1: // Z-Axis
        if (pos.z < m_median.z) m_left->Insert(object, depth - 1, dimensions);
        else m_right->Remove(object, depth - 1, dimensions);
        break;
    case 2: // Y-Axis
        if (pos.y < m_median.y) m_left->Insert(object, depth - 1, dimensions);
        else m_right->Remove(object, depth - 1, dimensions);
        break;
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

void KDTree::DebugDraw()
{
    m_root->DebugDraw();
}
