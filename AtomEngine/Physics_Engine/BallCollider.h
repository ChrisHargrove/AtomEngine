#pragma once

#include "Component.h"
#include "Collider.h"
#include "DebugCuboid.h"

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

class ATOM_API BallCollider : public Collider
{
public:
    friend class PhysicsManager;

    BallCollider();
    virtual ~BallCollider();

    bool Initialize() override;

protected:

    virtual void DrawDebug() override;

    float m_radius;

    friend class cereal::access;

    template <class Archive>
    void serialize(Archive &archive) {
    }

private:
    DebugCuboid* m_debugCube;

};

CEREAL_REGISTER_TYPE_WITH_NAME(BallCollider, "BallColliderObject");

