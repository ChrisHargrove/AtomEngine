#include "SortAndSweep.h"
#include "Transform.h"

#include "ThreadPool.h"

SortAndSweep::SortAndSweep()
{
}

SortAndSweep::~SortAndSweep()
{
}

void SortAndSweep::UpdateAABBList(const std::vector<RigidBody*>& bodies)
{
    auto SIZE_2 = 2 * bodies.size();
    //Get all colliders
    m_AABBList.resize(bodies.size());
    for(int i = 0; i < bodies.size(); i++) {
        m_AABBList[i] = bodies[i]->m_collider;
    }
    //Get all EndPoints
    if(m_XAxisEndPoints.size() != SIZE_2)
    {
        //Resize containers
        m_XAxisEndPoints.resize(SIZE_2);
        m_YAxisEndPoints.resize(SIZE_2);
        m_ZAxisEndPoints.resize(SIZE_2);
        //Get EndPoints
        int endPointIndex = 0;
        for(int i = 0; i < m_AABBList.size(); i++) {
            m_XAxisEndPoints[endPointIndex].Set(i, FLT_MAX);
            m_YAxisEndPoints[endPointIndex].Set(i, FLT_MAX);
            m_ZAxisEndPoints[endPointIndex].Set(i, FLT_MAX);
            endPointIndex++;
            m_XAxisEndPoints[endPointIndex].Set(i, FLT_MAX, false);
            m_YAxisEndPoints[endPointIndex].Set(i, FLT_MAX, false);
            m_ZAxisEndPoints[endPointIndex].Set(i, FLT_MAX, false);
            endPointIndex++;
        }
        //Setup lookup containers
        m_XLookUp.resize(SIZE_2);
        m_YLookUp.resize(SIZE_2);
        m_ZLookUp.resize(SIZE_2);
        for(int i = 0; i < m_XLookUp.size(); i++) {
            m_XLookUp[m_XAxisEndPoints[i].GetLookUp()] = i;
            m_YLookUp[m_XAxisEndPoints[i].GetLookUp()] = i;
            m_ZLookUp[m_XAxisEndPoints[i].GetLookUp()] = i;
        }
    }
}

const std::vector<std::pair<int, int>>& SortAndSweep::GenerateOverlapList()
{
    //Update All EndPoints To Be Correct
    UpdateEndPoints();

    std::vector<std::future<void>> jobCompletion;
    //Update All IntervalEvents
    for(int i = 0; i < 3; i++)
    {
        switch(i)
        {
        case 0:
            jobCompletion.push_back(JobSystem::Instance()->AddJob([this]()
            {
                UpdateIntervalEvents(m_XAxisEndPoints, m_XLookUp);
            }));
            break;
        case 1:
            jobCompletion.push_back(JobSystem::Instance()->AddJob([this]()
            {
                UpdateIntervalEvents(m_YAxisEndPoints, m_YLookUp);
            }));
            break;
        case 2:
            jobCompletion.push_back(JobSystem::Instance()->AddJob([this]()
            {
                UpdateIntervalEvents(m_ZAxisEndPoints, m_ZLookUp);
            }));
            break;
        default: ;
        }
        
    }

    while(!AreJobsReady(jobCompletion))
    {
        
    }

    /*UpdateIntervalEvents(m_XAxisEndPoints, m_XLookUp);
    UpdateIntervalEvents(m_YAxisEndPoints, m_YLookUp);
    UpdateIntervalEvents(m_ZAxisEndPoints, m_ZLookUp);*/

    //Generate Overlap Pairs
    for(auto evt : m_events)
    {
        if(evt->m_isRemoval) {
            m_overlapSet.erase(std::make_pair(evt->m_index0, evt->m_index1));
        }
        else {
            m_overlapSet.insert(std::make_pair(evt->m_index0, evt->m_index1));
        }
        delete evt;
    }
    m_events.clear();

    //Collect total overlaps
    m_overlapingList.clear();
    for(auto pair : m_overlapSet) {
        m_overlapingList.push_back(pair);
    }

    //return list
    return m_overlapingList;
}

void SortAndSweep::UpdateEndPoints()
{
    for(int i = 0; i < m_AABBList.size(); i++)
    {
        auto min = m_AABBList[i]->GetMinimumBounds();
        auto max = m_AABBList[i]->GetMaximumBounds();

        m_XAxisEndPoints[m_XLookUp[2 * i]].m_value = min.x;
        m_YAxisEndPoints[m_YLookUp[2 * i]].m_value = min.y;
        m_ZAxisEndPoints[m_ZLookUp[2 * i]].m_value = min.z;
        m_XAxisEndPoints[m_XLookUp[2 * i + 1]].m_value = max.x;
        m_YAxisEndPoints[m_YLookUp[2 * i + 1]].m_value = max.y;
        m_ZAxisEndPoints[m_ZLookUp[2 * i + 1]].m_value = max.z;
    }
}

void SortAndSweep::UpdateIntervalEvents(std::vector<EndPoint>& endpoints, std::vector<int>& lookup)
{

    std::unique_lock<std::mutex> intervalLock(mut);

    int i = 0;
    for(int ep = 0; ep < endpoints.size(); ++ep, ++i)
    {
        EndPoint key = endpoints[ep];
        int j = i - 1;
        while(j >= 0 && key < endpoints[j]) {
            EndPoint lhs = endpoints[j];
            EndPoint rhs = endpoints[j + 1];
            if(lhs.m_isStart) {
                if(!rhs.m_isStart) {
                    int lhs_index = lhs.m_index;
                    int rhs_index = rhs.m_index;
                    if(lhs_index != rhs_index) {
                        intervalLock.lock();
                        m_events.push_back(new IntervalEvent(lhs_index, rhs_index));
                        intervalLock.unlock();
                    }
                }
            }
            else {
                if(rhs.m_isStart) {
                    int lhs_index = lhs.m_index;
                    int rhs_index = rhs.m_index;
                    if(lhs_index != rhs_index && Overlapping(lhs_index, rhs_index, m_XLookUp, m_XAxisEndPoints) && Overlapping(lhs_index, rhs_index, m_YLookUp, m_YAxisEndPoints) && Overlapping(lhs_index, rhs_index, m_ZLookUp, m_ZAxisEndPoints)) {
                        intervalLock.lock();
                        m_events.push_back(new IntervalEvent(lhs_index, rhs_index, false));
                        intervalLock.unlock();
                    }
                }
            }
            endpoints[j] = rhs;
            endpoints[j + 1] = lhs;
            lookup[rhs.GetLookUp()] = j;
            lookup[lhs.GetLookUp()] = j + 1;
            --j;
        }
        endpoints[j + 1] = key;
        lookup[key.GetLookUp()] = j + 1;
    }
}

inline bool SortAndSweep::Overlapping(int lhs_index, int rhs_index, const std::vector<int>& lookup,
    const std::vector<EndPoint>& endpoints)
{
    float lhs_maximum = endpoints[lookup[2 * lhs_index + 1]].m_value;
    float rhs_minimum = endpoints[lookup[2 * rhs_index]].m_value;
    if(lhs_maximum < rhs_minimum) {
        return false;
    }
    float rhs_maximum = endpoints[lookup[2 * rhs_index + 1]].m_value;
    float lhs_minimum = endpoints[lookup[2 * lhs_index]].m_value;
    return lhs_minimum <= rhs_maximum;
}
