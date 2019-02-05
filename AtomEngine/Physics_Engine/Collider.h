#pragma once

#include "PhysicsTypes.h"

class Collider : public Component
{
public:
    Collider();
    virtual ~Collider();

    bool Initialize() override = 0;

    glm::vec3 GetMinimumBounds() const;
    glm::vec3 GetMaximumBounds() const;

protected:
    friend class PhysicsManager;

    virtual EndPoint GetStartPoint(const Axis axis) const = 0;
    virtual EndPoint GetEndPoint(const Axis axis) const = 0;

    virtual void DrawDebug() = 0;

    friend class cereal::access;
    template <class Archive>
    void serialize(Archive &archive) {
    }

protected:
    bool m_initialized;

    glm::vec3 m_minBounds;
    glm::vec3 m_maxBounds;

};

