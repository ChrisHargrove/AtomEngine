#pragma once
#include "BroadPhase.h"
#include <set>
#include "AABB.h"
#include <SDL/SDL_syswm.h>

/*!
 * \class BVHNode "BoundingVolumeHeirarchy.h"
 * \brief Node structure for each part of the Bounding Volume Tree.
 * 
 * Allows for the tree structure to be built as each node has a link to parent and child nodes.
 * Each node also has its own AABB that contains child nodes AABB's, and inside the leaf nodes will
 * contain the actual AABB's of objects in the simulation.
 * The nodes AABB will contain child nodes inside a tight fitting box, but if the node is a leaf padding is applied
 * so that it is larger than the AABB it contains so there must be significant movement of the object before an update is applied.
 */
class BVHNode
{
public:
    //Allow the overall tree structure access to the nodes.
    friend class BoundingVolumeHeirarchy;

    /*!
     * \brief Default Constructor
     */
    BVHNode() :
        m_parent(nullptr),
        m_objectAABB(nullptr)
    {        
        m_childNodes[0] = nullptr;
        m_childNodes[1] = nullptr;
    }

    /*!
     * \brief Default Destructor
     */
    ~BVHNode()
    {
        if (m_childNodes[0]) delete m_childNodes[0];
        if (m_childNodes[1]) delete m_childNodes[1];
    }

    /*!
     * \brief Test if node is leaf node or not.
     */
    bool IsLeaf() const {
        return m_childNodes[0] == nullptr;
    }

    /*!
     * \brief Clears all pointers from the node for safe deletion.
     */
    void Clear() {
        m_childNodes[0] = nullptr;
        m_childNodes[1] = nullptr;
        m_parent = nullptr;
    }

    /*!
     * \brief Turns the node into a branch node
     * \param n0 Child Node Pointer to add to this node.
     * \param n1 Child Node Pointer to add to this node.
     * 
     * Adds to children node to this node, sets each of their parents to this node.
     */
    void MakeNodeIntoBranch(BVHNode* n0, BVHNode* n1) {
        n0->m_parent = this;
        n1->m_parent = this;

        m_childNodes[0] = n0;
        m_childNodes[1] = n1;
    }

    /*!
     * \brief Turns the node into a leaf node.
     * \param data The object AABB to be contained inside this node.
     * 
     * Stores the object AABB data inside this node. then sets the AABB containing node pointer
     * to this node.
     * After this sets both child nodes to nullptr.
     */
    void MakeNodeIntoLeaf(AABB* data) {
        m_objectAABB = data;
        data->m_containingNode = this;

        m_childNodes[0] = nullptr;
        m_childNodes[1] = nullptr;
    }

    /*!
     * \brief Updates the nodes AABB
     * \param margin How much gap to add around the contained object for the nodes AABB
     * 
     * If the node is a leaf node, adds the margin as padding around the object.
     * Else it will merge the two child nodes AABB returning a perfect fitting AABB of the child nodes.
     */
    void UpdateAABB(float margin)
    {
        if(IsLeaf()) {
            const glm::vec3 marginVector(margin);
            m_nodeAABB.m_minBounds = m_objectAABB->m_minBounds - marginVector;
            m_nodeAABB.m_maxBounds = m_objectAABB->m_maxBounds + marginVector;
        }else {
            m_nodeAABB = AABB::MergeAABB(m_childNodes[0]->m_nodeAABB, m_childNodes[1]->m_nodeAABB);
        }
    }

    /*!
     * \brief Gets the sibling node of the current node.
     * \return Returns a BVHNode pointer to the sibling node.
     * 
     * Gets the parent node and tests which of its child nodes are not equal to this node.
     * Then returns it.
     */
    BVHNode* GetSibling() const {
        if(this == m_parent->m_childNodes[0]) {
            return m_parent->m_childNodes[1];
        }
        else {
            return m_parent->m_childNodes[0];
        }
    }

protected:
    BVHNode* m_parent;              /*!< Pointer to the parent node.*/
    BVHNode* m_childNodes[2] = {};  /*!< Two component Array of nodes to children.*/

    AABB m_nodeAABB;                /*!< This nodes AABB.*/
    AABB* m_objectAABB;             /*!< Pointer to the contained objects AABB.*/

    bool m_childrenChecked = false; /*!< Flag to see if this node has been checked during queries.*/

};


/*!
 * \class BoundingVolumeHeirarchy "BoundingVolumeHeirarchy.h"
 * \brief Data Structure for Axis-Aligned Bounding Boxes, that stores them in a tree structure.
 *
 * Handles the storage of all AABB's in simulation including insertion, deletion and updating them.
 * Allows for collision queries of AABB's vs AABB's in the tree, by testing the AABB against the tree nodes.
 */
class BoundingVolumeHeirarchy : public BroadPhase
{
public:
    /*!
     * \brief Default Constructor
     */
    BoundingVolumeHeirarchy (DebugRenderer* debugRenderer): BroadPhase(debugRenderer), m_root(nullptr)
    {
        m_debugRenderer = debugRenderer;
        m_debugCuboid.SetColor(glm::vec3(1, 1, 1));
    }

    /*!
     * \brief Default Destructor
     */
    virtual ~BoundingVolumeHeirarchy() = default;

    /*!
     * \brief Removes an AABB from the tree.
     * \param aabb The AABB to remove.
     * 
     * Gets the containing node from the "aabb" parameter, and casts it to a BVHNode.
     * After this sets that node AABB to nullptr, and the "aabb" containing node to nullptr.
     * Then removes the node from the tree.
     */
    void Remove(AABB* aabb) override {
        BVHNode* node = static_cast<BVHNode*>(aabb->m_containingNode);

        node->m_objectAABB = nullptr;
        aabb->m_containingNode = nullptr;

        RemoveNode(node);
    }

    /*!
     * \brief Removes a node from the tree.
     * \param node The node to remove from the tree.
     * 
     * Removes a BVHNode from the tree.
     */
    void RemoveNode(BVHNode* node) {
        //Remove node from node list
        m_nodeList.remove(node);
        //First get the parent node, of node being removed.
        BVHNode* parent = node->m_parent;
        //If there is a parent
        if(parent) {
            //Get the sibling of the node being removed.
            BVHNode* sibling = node->GetSibling();
            //If the parent has a parent.
            if(parent->m_parent) {
                //Set the sibling nodes parent to the grandparent.
                sibling->m_parent = parent->m_parent;
                //Find which of the grandparents nodes is the parent.
                //And replace with sibling
                if(parent == parent->m_parent->m_childNodes[0]) {
                    parent->m_parent->m_childNodes[0] = sibling;
                }
                else {
                    parent->m_parent->m_childNodes[1] = sibling;
                }
            }
            //If no grandparent then parent must be root node.
            else {
                //Set the root node to be the sibling.
                m_root = sibling;
                //Set sibling node to have to parent.
                sibling->m_parent = nullptr;
            }
            //Delete the node being removed.
            node->Clear();
            delete node;
            m_nodeList.remove(node);
            //Delete the obsolete parent node.
            parent->Clear();
            delete parent;
            m_nodeList.remove(node);
        }
        //If node parent node, the this node must be root node.
        else {
            //Set root node to nullptr.
            m_root = nullptr;
            //Delete this node.
            delete node;
            //Tree is empty at this point.
        }
    }

    /*!
     * \brief Will clear the tree of all nodes.
     * 
     * Removes all nodes from the tree by deleting root node, this will propagate down tree deleting all nodes.
     */
    void Clear() override {
        delete m_root;
    }

    /*!
     * \brief Updates Tree.
     * 
     * Updates the AABB tree so that all AABB's are in the correct parent and of the right size.
     */
    void Update() override {
        //If root node exists.
        if(m_root) {
            //Is the root the only node
            if(m_root->IsLeaf()) {
                //Update the AABB with margin.
                m_root->UpdateAABB(m_margin);
            }
            //If root is not only node
            else {
                //Grab all nodes requiring update
                m_invalidNodes.clear();
                FindInvalidNodes(m_root, m_invalidNodes);

                //Reinsert all invalid nodes into tree;
                for(BVHNode* node : m_invalidNodes) {
                    //Get parent of invalid node.
                    BVHNode* parent = node->m_parent;
                    //Get sibling of invalid node.
                    BVHNode* sibling = node->GetSibling();

                    BVHNode** parentLink;
                    //If there is a grandparent.
                    if(parent->m_parent) {
                        //Find which of grandparents children is the parent.
                        if(parent == parent->m_parent->m_childNodes[0]) {
                            //set the parentLink to be that grandparents child.
                            parentLink = &parent->m_parent->m_childNodes[0];
                        }
                        else {
                            parentLink = &parent->m_parent->m_childNodes[1];
                        }
                        //Set the siblings parent to the grandparent.
                        sibling->m_parent = parent->m_parent;
                    }
                    else {
                        //If no grandparent then the new parent needs to be the root.
                        parentLink = &m_root;
                        //And siblings parent needs to be nullptr.
                        sibling->m_parent = nullptr;
                    }

                    //Make the parentLink the sibling
                    *parentLink = sibling;
                    //At this point we have removed the node from the tree without deletion
                    //And the sibling is the new parent node.
                    //Now the old parent is cleared and deleted as its now useless.
                    parent->Clear();
                    delete parent;
                    m_nodeList.remove(parent);

                    //Set nodes parent to nullptr
                    node->m_parent = nullptr;
                    //Update the node so its size is correct for either its child nodes or contained object.
                    node->UpdateAABB(m_margin);
                    //Reinsert the node at the top of the tree so it finds its correct place.
                    //m_nodeList.remove(node);
                    InsertNode(node, &m_root);

                }
                //Clear all invalid nodes as they are no longer needed.
                m_invalidNodes.clear();
            }
        }

        if (m_showBVHDebug) {
            //Add all nodes in node list to debug renderer
            auto aabb = m_root->m_nodeAABB;
            auto pos = aabb.m_minBounds + aabb.GetExtents() * 0.5f;

            glm::mat4 trans = glm::mat4(1.0f);
            trans = glm::translate(trans, pos);
            trans = glm::scale(trans, aabb.GetExtents());
            m_debugRenderer->AddToBuffer(&m_debugCuboid, m_debugCuboid.GetColor(), trans);

            for (auto node : m_nodeList)
            {
                aabb = node->m_nodeAABB;
                pos = aabb.m_minBounds + aabb.GetExtents() * 0.5f;

                trans = glm::mat4(1.0f);
                trans = glm::translate(trans, pos);

                trans = glm::scale(trans, aabb.GetExtents());
                m_debugRenderer->AddToBuffer(&m_debugCuboid, m_debugCuboid.GetColor(), trans);
            }
        }
    }

    /*!
     * \brief Add an AABB to the Bounding Tree
     * \param aabb A pointer to the new aabb to add to the tree.
     * 
     * Adds AABB's into the bounding tree, by first creating a new node and then adding the new object into it.
     * Makes the new Node fatter so there is a amount of wiggle room for the objects to move before the tree updates.
     */
    void Add(AABB* aabb) override {
        //If there is a root node.
        if(m_root) {
            //Create a new node
            BVHNode* node = new BVHNode();
            //Make it a leaf that contains the new "aabb"
            node->MakeNodeIntoLeaf(aabb);
            //Make the node AABB fatter to fit the "aabb"
            node->UpdateAABB(m_margin);
            //Insert the new node at the top of the tree to find correct spot.
            InsertNode(node, &m_root);
        }
        //If tree is empty
        else {
            //Create the root node
            m_root = new BVHNode();
            //Make it a leaf with "aabb"
            m_root->MakeNodeIntoLeaf(aabb);
            //Make it fatter 
            m_root->UpdateAABB(m_margin);
        }
    }

    /*!
     * \brief Inserts a node into the Bounding Tree.
     * \param node A pointer to the node you wish to insert.
     * \param parent A double pointer to the parent node.
     * 
     * Inserts a node into the tree at the specified parent node.
     * If the parent is not a leaf not, it will test both sides of the children
     * to see which will have a smaller increase of volume after insertion and chooses
     * that one as the parent for the inserted node.
     */
    void InsertNode(BVHNode* node, BVHNode** parent)
    {
        //Get parent from double pointer.
        BVHNode* p = *parent;
        //If the parent is a leaf
        if(p->IsLeaf()) {
            //Make a new node.
            //And swap its place with parent.
            //Then insert both parent and new node into the new parent.
            BVHNode* newParent = new BVHNode();
            newParent->m_parent = p->m_parent;
            newParent->MakeNodeIntoBranch(node, p);
            *parent = newParent;
            if (m_showBVHDebug) {
                m_nodeList.push_back(newParent);
            }
        }
        //If the node inserted into is a branch.
        else {
            //Get both child nodes AABB's
            AABB aabb0 = p->m_childNodes[0]->m_nodeAABB;
            AABB aabb1 = p->m_childNodes[1]->m_nodeAABB;

            //Calculate the volume difference after inserting new node into both children.
            const float volumeDiff1 = AABB::MergeAABB(aabb0, node->m_nodeAABB).GetVolume() - aabb0.GetVolume();
            const float volumeDiff2 = AABB::MergeAABB(aabb1, node->m_nodeAABB).GetVolume() - aabb1.GetVolume();

            //Choose child node with the smallest volume increase and insert node into it.
            if(volumeDiff1 < volumeDiff2) {
                InsertNode(node, &p->m_childNodes[0]);
            }
            else {
                InsertNode(node, &p->m_childNodes[1]);
            }
        }

        //After all Insertion is done, Will recursively exit the tree while updating all AABB's that
        //were modified to fit properly.
        (*parent)->UpdateAABB(m_margin);
        
    }

    /*!
     * \brief Calculates The Collision Pairs.
     * \return Returns a list of all collision pairs found.
     */
    CollisionPairList CalculatePairs() override {
        //Clear all collision pairs.
        m_collisionPairs.clear();
        //Reset Checks made.
        m_checksMade = 0;
        //If root does not exist or it is a leaf
        //return empty collision list.
        if(!m_root || m_root->IsLeaf()) {
            return m_collisionPairs;
        }
        //Reset all nodes checked flag.
        ClearCheckedFlags(m_root);

        //Find all collision pairs starting from first two child nodes in tree.
        FindPairs(m_root->m_childNodes[0], m_root->m_childNodes[1]);

        //Loop through all pairs and set their collision status
        for(auto pair : m_collisionPairs) {
            pair.first->IsColliding() = AABB::POTENTIAL;
            pair.second->IsColliding() = AABB::POTENTIAL;
        }

        //Return all found pairs.
        return m_collisionPairs;
    }

    /*!
     * \brief Gets the number of checks made this frame.
     * \return Returns the number of checks made this frame.
     */
    int GetChecksMade() override {
        return m_checksMade;
    }

    /*!
     * \brief Sets whether the debug rendering should show or not.
     * \param enable Should debug rendering be enabled.
     */
    bool* GetShowDebug() override {
        return &m_showBVHDebug;
    }
    
private:
    BVHNode* m_root;        /*!< The root node of the bounding tree.*/
    float m_margin = 0.5f;  /*!< The margin to apply around all leaf nodes.*/
    int m_checksMade = 0;   /*!< A counter for how many actual checks were performed during this broadphase.*/
    bool m_showBVHDebug = false;

    CollisionPairList m_collisionPairs;     /*!< The list of collision pairs found.*/
    std::vector<BVHNode*> m_invalidNodes;   /*!< The list of invalid nodes found.*/
    std::list<BVHNode*> m_nodeList;       /*!< List of all nodes in tree.*/

    /*!
     * \brief Finds all node that are requiring reinsertion into the tree.
     * \param node The node to check for invalid AABB's
     * \param invalidNodes Reference to invalidNode storage.
     * 
     * Recursively checks all nodes starting from the one specified. Will check to see if the object
     * AABB still fits inside the Nodes AABB, if not then the node need to be reinserted.
     */
    static void FindInvalidNodes(BVHNode* node, std::vector<BVHNode*>& invalidNodes) {
        //Is node a leaf
        if(node->IsLeaf()) {
            //Does object node still fit inside Node AABB.
            if(!node->m_nodeAABB.Contains(*node->m_objectAABB)) {
                //Add node to list.
                invalidNodes.push_back(node);
            }
        }
        //If node is branch.
        else {
            //Check both child nodes for invalidNodes.
            FindInvalidNodes(node->m_childNodes[0], invalidNodes);
            FindInvalidNodes(node->m_childNodes[1], invalidNodes);
        }
    }

    /*!
     * \brief Sets Checked Flag of all nodes in tree to false.
     * \param node Node to start the operation from.
     * 
     * Recursively traverses the tree setting all nodes checked flag to false.
     * More of a helper function.
     */
    static void ClearCheckedFlags(BVHNode* node)
    {
        //Set node checked to false.
        node->m_childrenChecked = false;
        //If node has children.
        if(!node->IsLeaf()) {
            //Set child nodes to have flag set false.
            ClearCheckedFlags(node->m_childNodes[0]);
            ClearCheckedFlags(node->m_childNodes[1]);
        }
    }

    /*!
     * \brief Finds Colliding Pairs starting at the nodes specified.
     * \param n0 Pointer to first node to start at.
     * \param n1 Pointer to second node to start at.
     * 
     * Recursively checks the two nodes given against each other for colliding pairs.
     */
    void FindPairs(BVHNode* n0, BVHNode* n1)
    {
        //If first node is a leaf.
        if(n0->IsLeaf()) {
            //If second node is a leaf.
            if(n1->IsLeaf()) {
                //Check for a collision of the object AABB's
                m_checksMade++;

                if(n0->m_objectAABB->Collides(n1->m_objectAABB)) {
                    //If they do collide add them to the collision pair list.
                    m_collisionPairs.push_back(std::make_pair(n0->m_objectAABB, n1->m_objectAABB));
                }
            }
            //If second node is not a leaf.
            else {
                //Check second nodes children against each other.
                CheckChildren(n1);
                //Check first node with second nodes children nodes only if the 2 nodes collide.
                if(n0->m_nodeAABB.Collides(&n1->m_nodeAABB)) {
                    FindPairs(n0, n1->m_childNodes[0]);
                    FindPairs(n0, n1->m_childNodes[1]);
                }
                
            }
        }
        //If first node not a leaf.
        else {
            //If second node is a leaf.
            if(n1->IsLeaf())
            {
                //Check first nodes children against each other.
                CheckChildren(n0);
                //Check first nodes children with second node.
                if(n1->m_nodeAABB.Collides(&n0->m_nodeAABB)) {
                    FindPairs(n0->m_childNodes[0], n1);
                    FindPairs(n0->m_childNodes[1], n1);
                }
                
            }
            //If both first and second nodes are branches
            else {
                //Check both first and second nodes children against themselves.
                CheckChildren(n0);
                CheckChildren(n1);
                //Check first and second nodes children against each other.
                if(n0->m_nodeAABB.Collides(&n1->m_nodeAABB)) {
                    FindPairs(n0->m_childNodes[0], n1->m_childNodes[0]);
                    FindPairs(n0->m_childNodes[0], n1->m_childNodes[1]);
                    FindPairs(n0->m_childNodes[1], n1->m_childNodes[0]);
                    FindPairs(n0->m_childNodes[1], n1->m_childNodes[1]);
                }
                

            }
        }

    }

    /*!
     * \brief Checks both child nodes of node specified against each other.
     * \param node A pointer to the node whose children you wish to check.
     * 
     * A utility function that checks if a node has already been checked, and will
     * attempt to find collision pairs from both child nodes of the node specified.
     */
    void CheckChildren(BVHNode* node) {
        //Has node already been checked
        if(!node->m_childrenChecked) {
            //Find Pairs from both nodes.
            FindPairs(node->m_childNodes[0], node->m_childNodes[1]);
            //Mark the node as checked.
            node->m_childrenChecked = true;
        }
    }

};
