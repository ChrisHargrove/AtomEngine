#pragma once

#include "BroadPhase.h"
#include "Collider.h"

class EndPoint
{
public:
    friend class SortAndSweep;

    EndPoint() : 
        m_index(0), m_value(0), m_isStart(false) {}

    void Set(int index, float value, bool isStart = true) {
        m_index = index;
        m_value = value;
        m_isStart = isStart;
    }

    int GetIndex() const {
        return m_index;
    }

    bool IsStart() const {
        return m_isStart;
    }

    int GetLookUp() const {
        return 2 * m_index + (m_isStart ? 0 : 1);
    }

    bool operator < (const EndPoint& other) const {
        if(m_value == other.m_value) {
            return m_isStart < other.m_isStart;
        }else {
            return m_value < other.m_value;
        }
    }

protected:
    int m_index;
    float m_value;
    bool m_isStart;
};

class IntervalEvent
{
public:
    friend class SortAndSweep;

    IntervalEvent(int index0, int index1, bool isRemoval = true) :
        m_index0(index0), m_index1(index1), m_isRemoval(isRemoval) {}

    bool IsRemoval() const {
        return m_isRemoval;
    }

    int GetIndex0() const {
        return m_index0;
    }

    int GetIndex1() const {
        return m_index1;
    }

protected:
    int m_index0;
    int m_index1;
    bool m_isRemoval;
};

class SortAndSweep : public BroadPhase
{
public:
    SortAndSweep();
    virtual ~SortAndSweep();

    void UpdateAABBList(const std::vector<RigidBody*>& bodies) override;
    const std::vector<std::pair<int, int>>& GenerateOverlapList() override;

private:
    std::vector<Collider*> m_AABBList;
    std::vector<EndPoint> m_XAxisEndPoints;
    std::vector<EndPoint> m_YAxisEndPoints;
    std::vector<EndPoint> m_ZAxisEndPoints;
    std::vector<int> m_XLookUp;
    std::vector<int> m_YLookUp;
    std::vector<int> m_ZLookUp;

    std::vector<IntervalEvent*> m_events;
    std::set<std::pair<int, int> > m_overlapSet;
    std::vector<std::pair<int, int> > m_overlapingList;

    void UpdateEndPoints();
    void UpdateIntervalEvents(std::vector<EndPoint>& endpoints, std::vector<int>& lookup);
    inline bool Overlapping(int lhs_index, int rhs_index, const std::vector<int>& lookup, const std::vector<EndPoint>& endpoints);
};

