#include "PhysicsManager.h"
#include <algorithm>
#include "BoxCollider.h"
#include "BallCollider.h"
#include "ThreadPool.h"

void PhysicsManager::Initialize()
{
    m_kdTree = new KDTree(m_bodies, 3, 3);
    m_drawDebug = false;
    m_drawColliders = false;
    m_drawKDTree = false;
    m_gravityMultiplier = 1.0f; //How many times earths gravity.
    m_gravityAcceleration = -9.81; //Acceleration Like on earth
}

void PhysicsManager::Shutdown()
{
}

void PhysicsManager::Reset()
{
    m_bodies.clear();
}

void PhysicsManager::StepSimulation(float deltaTime)
{
    if (m_stepSimulation)
    {
        if (!m_bodies.empty())
        {
            auto numThreads = JobSystem::Instance()->m_numThreads;
            auto numObj = m_bodies.size() / numThreads;
            if (numObj == 0) {
                numObj = m_bodies.size() % numThreads;
            }

            std::vector<RigidBody*>::iterator start;
            std::vector<RigidBody*>::iterator end;

            std::vector<std::future<void>> jobCompletion;

            bool skipUnnecessary = false;

            for (int i = 0; i < numThreads; i++)
            {
                if (skipUnnecessary) continue;

                if (i == 0) { start = m_bodies.begin(); }
                else if (numObj < numThreads) { start = m_bodies.begin(); skipUnnecessary = true; }
                else { start = std::next(m_bodies.begin(), numObj * i); }

                if (i == numThreads - 1) { end = m_bodies.end(); }
                else if (numObj < numThreads) { end = m_bodies.end(); }
                else { end = std::next(m_bodies.begin(), numObj * (i + 1)); }

                auto shortBodyList = std::vector<RigidBody*>(start, end);
                auto gravMult = m_gravityMultiplier;
                auto gravAccel = m_gravityAcceleration;

                auto job = [shortBodyList, deltaTime, gravMult, gravAccel]() {
                    for (auto& rb : shortBodyList) {
                        if (rb->UsesGravity()) {
                            rb->ApplyForce(glm::vec3(0, (gravAccel * gravMult * rb->GetMass()), 0) * deltaTime);
                        }
                        rb->Simulate(deltaTime);
                    }
                };

                if (skipUnnecessary) {
                    jobCompletion.push_back(JobSystem::Instance()->AddJob(job));
                }
                else if (i != numThreads - 1) {
                    jobCompletion.push_back(JobSystem::Instance()->AddJob(job));
                }
                else {
                    jobCompletion.push_back(JobSystem::Instance()->AddJob(job));
                }

            }
            while (!AreJobsReady(jobCompletion)) {};
        }
    }
    //if(m_kdTree) m_kdTree->Rebuild();
}

void PhysicsManager::DrawDebug()
{
    if(m_drawDebug)
    {
        if(m_drawColliders)
        {
            for (auto body : m_bodies)
            {
                Collider* collider = body->GetComponent<BoxCollider>();
                if (collider != nullptr)
                {
                    collider->DrawDebug();
                }
                collider = body->GetComponent<BallCollider>();
                if (collider != nullptr)
                {
                    collider->DrawDebug();
                    return;
                }
            }
        }
        
        if(m_drawKDTree)
        {
            if (m_kdTree)
            {
                m_kdTree->DebugDraw();
            }
        }
        
    }
    
}

void PhysicsManager::AddBody(RigidBody* body)
{
    m_bodies.push_back(body);
    if (m_kdTree) {
        m_kdTree->Insert(body);
    }
}

void PhysicsManager::RemoveBody(RigidBody* body)
{
    m_bodies.erase(std::remove_if(m_bodies.begin(), m_bodies.end(),
        [body](RigidBody* rbody) {return rbody == body; }), m_bodies.end());
    if (m_kdTree) {
        m_kdTree->Remove(body);
    }
}

bool& PhysicsManager::StartSimulation()
{
    return m_stepSimulation;
}

bool& PhysicsManager::ShouldDrawDebug()
{
    return m_drawDebug;
}

bool& PhysicsManager::ShouldDrawColliders()
{
    return m_drawColliders;
}

bool& PhysicsManager::ShouldDrawKDTree()
{
    return m_drawKDTree;
}

