#pragma once
#include "AABB.h"
#include "ProfilerManager.h"

/*!
 * \class NarrowPhase "NarrowPhase.h"
 * \brief Base class for all Narrow Phase collision stages.
 * 
 * Sets up the base class for all Narrow Phase collisions, with the mandatory functions required.
 */
class NarrowPhase

{
public:
    /*!
     * \brief Default Constructor
     */
    NarrowPhase(){}

    /*!
     * \brief Default Destructor
     */
    virtual ~NarrowPhase(){}

    /*!
     * \brief Gets all the collision pairs
     * \param potentials List of all potential collisions from the broadphase.
     * 
     * Runs the narrow phase collision detection on the list of collision pairs provided by
     * the broad phase.
     */
    virtual CollisionPairList GetCollisions(const CollisionPairList& potentials) = 0;

};

/*!
 * \class GJK "NarrowPhase.h"
 * \brief Gilbert-Johnson-Keerthi Algorithm
 * 
 * Calculates whether the collision pairs are actually colliding or not by using Minkowski Differences to form support points.
 */
class GJK : public NarrowPhase
{
public:
    /*!
     * \brief Default Constructor
     */
    GJK(){}

    /*!
     * \brief Default Destructor.
     */
    ~GJK(){}

    /*!
     * \brief Gets all the collision pairs that are actually colliding.
     * \param potentials List of all potential collisions from the broad phase.
     * 
     * Creates a 4 point simplex (tetrahedron) and checks if it contains the origin.
     * If it does then the 2 objects must be colliding.
     */
    CollisionPairList GetCollisions(const CollisionPairList& potentials) override
    {
        CollisionPairList ammendedList;

        for(auto& pair : potentials) {
            if(EvolveSimplex(*pair.first, *pair.second)) {
                pair.first->IsColliding() = AABB::COLLIDING;
                pair.second->IsColliding() = AABB::COLLIDING;
            }
        }

        return ammendedList;
    }

    /*!
     * \brief Creates a 4 point simplex (tetrahedron) over many iterations.
     * \param box0 First Collider.
     * \param box1 Second Collider.
     * 
     * Creates a simplex, and evaluates where the origin is in relation. If the tetrahedron contains the origin
     * then the two colliders must be touching.
     */
    bool EvolveSimplex(const AABB& box0, const AABB& box1)
    {
        //Starting Iterations
        int iterationsDone = 0;
        //Create the first support point from the MinkowskiDifference, in the direction of seperation.
        auto support = MinkowskiDifferenceSupport(box0, box1, box0.m_transform->GetPosition() - box1.m_transform->GetPosition());
        //List of all support points.
        std::vector<glm::vec3> supports;
        //Add the first support point to the list.
        //This is point A.
        supports.push_back(support);
        //Reverse the diretion vector.
        m_direction = -support;

        Profiler::Instance()->Start("GJK");
        //Start the loop, only run until the max iterations.
        while(iterationsDone < m_maxIterations) {
            //Get next support point in the new direction.
            auto a = MinkowskiDifferenceSupport(box0, box1, m_direction);
            //Check if this new support point is in the same direction as the origin. If not early out as
            //cant be colliding.
            if (glm::dot(a, m_direction) < 0) return false;
            //Add the new support to the list. This is the new point A.
            supports.push_back(a);
            //Start Evaluating the Simplex.
            if (EvaluateSimplex(supports, m_direction)) return true;
            //Increase the iteration count.
            iterationsDone++;
        }
        Profiler::Instance()->End("GJK");
        //If while loop has finished return if max iterations was hit or not.
        return iterationsDone < m_maxIterations;

    }

    /*!
     * \brief Evaluates the simplex and sorts the vertex order and direction to check.
     * \param supports List of current simplex supports.
     * \param direction The direction to check for the new support.
     */
    bool EvaluateSimplex(std::vector<glm::vec3>& supports, glm::vec3& direction)
    {
        //Switch on the number of current supports.
        switch(supports.size())
        {
        case 2:
            {
                //Create a line from point A to point B.
                auto lineAB = supports[0] - supports[1];
                //Create a line from point A to origin.
                auto lineAO = -supports[1];
                //If line AB is in same direction as origin.
                if (glm::dot(lineAB, lineAO) > 0) {
                    //Set the new direction to the triple cross product.
                    //This gives a direction that is perpendicular to AB and AO in the direction of AB.
                    direction = glm::TripleProduct(lineAB, lineAO, lineAB);
                }
                //If not, set simplex to just point A and set direction to origin.
                else {
                    //Clear all old supports.
                    supports.clear();
                    //Set support to just point A.
                    supports.push_back(-lineAO);
                    direction = lineAO;
                }
                return false;
            }
            break;
        case 3:
            {
                //Create line from point A to origin.
                auto lineAO = -supports[2];
                //Create line from point A to point B.
                auto lineAB = supports[1] - supports[2];
                //Create line from point A to point C.
                auto lineAC = supports[0] - supports[2];
                //Create line that is perpendicular to the triangle ABC.
                auto lineABC = glm::cross(lineAB, lineAC);

                //Check if origin is on outside of edge AC.
                if(glm::dot(glm::cross(lineABC, lineAC), lineAO) > 0) {
                    //Check if origin is in same direction as line AC.
                    if(glm::dot(lineAC, lineAO) > 0) {
                        //Set the supports to be just A and C.
                        std::vector<glm::vec3> newSupports;
                        newSupports.push_back(supports[0]); //Point C
                        newSupports.push_back(supports[2]); //Point A
                        supports = newSupports;

                        //Set direction to be perpendicular to AC and AO but in the direction of AC.
                        direction = glm::TripleProduct(lineAC, lineAO, lineAC);
                        return false;
                    }
                    else {
                        //Check if AB is in the direction of origin.
                        if (glm::dot(lineAB, lineAO) > 0) {
                            //Set the new points to be just A and B
                            std::vector<glm::vec3> newSupports;
                            newSupports.push_back(supports[1]); //Point B
                            newSupports.push_back(supports[2]); //Point A
                            supports = newSupports;

                            //Set direction to be perpendicular to line AB and AB but in direction of AB.
                            direction = glm::TripleProduct(lineAB, lineAO, lineAB);
                            return false;
                        }
                        else {
                            //Closest point to origin is just point A
                            auto pointA = -lineAO;
                            //Set the only support to A
                            supports.clear();
                            supports.push_back(pointA);
                            //Set the search direction to be towards the origin.
                            direction = lineAO;
                            return false;
                        }
                    }
                }
                else {
                    //Check if origin is outside edge AB
                    if(glm::dot(glm::cross(lineAB, lineABC), lineAO) > 0) {
                        //Check if line AB is in the direction of origin.
                        if (glm::dot(lineAB, lineAO) > 0) {
                            //Set the support points to be A and B.
                            std::vector<glm::vec3> newSupports;
                            newSupports.push_back(supports[1]); //Point B.
                            newSupports.push_back(supports[2]); //Point A.
                            supports = newSupports;

                            //Set direction to be perpendicular to AB and AO in the direction of AB.
                            direction = glm::TripleProduct(lineAB, lineAO, lineAB);
                            return false;
                        }
                        else {
                            //Closest point to origin is A
                            auto pointA = -lineAO;
                            //Set the only support point to A.
                            supports.clear();
                            supports.push_back(pointA);
                            //Set the search direction to be towards the origin.
                            direction = lineAO;
                            return false;
                        }
                    }
                    else {
                        //Check if the origin is above the triangle ABC or not.
                        if(glm::dot(lineABC, lineAO) > 0) {
                            //Set search direction to be above triangle.
                            direction = lineABC;

                            return false;
                        }
                        else {
                            //If below the triangle change the winding order of the vertices.
                            //To be BCA.
                            std::vector<glm::vec3> newSupports;
                            newSupports.push_back(supports[1]); //Point B.
                            newSupports.push_back(supports[0]); //Point C.
                            newSupports.push_back(supports[2]); //Point A.
                            supports = newSupports;
                            //Set direction to be below the triangle.
                            direction = -lineABC;
                            return false;
                        }
                    }
                }
            }
            return false;
            break;
        case 4:
            {
                //Create line towards origin.
                auto lineAO = -supports[3];
                //Create line from A to B.
                auto lineAB = supports[2] - supports[3];
                //Create line from A to C.
                auto lineAC = supports[1] - supports[3];
                //Create line from A to D.
                auto lineAD = supports[0] - supports[3];

                //Get line perpendicular to ABC triangle.
                auto lineABC = glm::cross(lineAB, lineAC);
                //Get line perpendicular to ADB triangle.
                auto lineADB = glm::cross(lineAD, lineAB);
                //Get line perpendicular to ACD triangle.
                auto lineACD = glm::cross(lineAC, lineAD);

                //Check if origin is outside ABC triangle.
                if(glm::dot(lineABC, lineAO) > 0) {
                    //Set supports to be just triangle ABC.
                    std::vector<glm::vec3> newSupports;
                    newSupports.push_back(supports[1]); //Point C
                    newSupports.push_back(supports[2]); //Point B
                    newSupports.push_back(supports[3]); //Point A
                    supports = newSupports;

                    //Set search direction to be above triangle.
                    direction = lineABC;
                    return false;
                }
                //Check if origin is outside ADB triangle.
                else if (glm::dot(lineADB, lineAO) > 0) {
                    //Set supports to be just triangle ADB.
                    std::vector<glm::vec3> newSupports;
                    newSupports.push_back(supports[0]); //Point D.
                    newSupports.push_back(supports[2]); //Point B.
                    newSupports.push_back(supports[3]); //Point A.
                    supports = newSupports;

                    //Set search direction to be above triangle.
                    direction = lineADB;
                    return false;
                }
                //Check if origin is outside ACD triangle.
                else if (glm::dot(lineACD, lineAO) > 0) {
                    //Set supports to be just triangle ACD.
                    std::vector<glm::vec3> newSupports;
                    newSupports.push_back(supports[0]); //Point D.
                    newSupports.push_back(supports[1]); //Point C.
                    newSupports.push_back(supports[3]); //Point A
                    supports = newSupports;

                    //Set search direction to be above triangle.
                    direction = lineACD;
                    return false;
                }else {
                    //If this stage is reached the Simplex encompasses the origin.
                    return true;
                }
            }
            break;
        default:
            //This stage should never be reached. If it is just default to false.
            return false;
            break;
        }
    }


private:
    int m_maxIterations = 100; /*! The maximum iterations GJK will make before it aborts.*/
    glm::vec3 m_direction = glm::vec3(0.0f);    /*!< The Search direction for the simplex.*/

    /*!
     * \brief Gets a support point from the Minkowski Difference.
     * \param box0 The first collider as reference.
     * \param box1 The second collider as reference.
     * \param direction The search direction as reference.
     * 
     * Uses the Minkowski difference of both colliders and the supplied direction vector to get a support
     * point for the Simplex.
     */
    glm::vec3 MinkowskiDifferenceSupport(const AABB& box0, const AABB& box1, const glm::vec3& direction) {
        //Get the first support from second collider.
        auto supportA = box1.m_mesh->Support(*box1.m_transform, direction);
        //Get the second support from first collider.
        auto supportB = box0.m_mesh->Support(*box0.m_transform, -direction);
        //Get the difference of the two supports.
        return glm::vec3(supportA - supportB);
    }

};
