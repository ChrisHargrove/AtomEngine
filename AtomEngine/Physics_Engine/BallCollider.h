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

    virtual EndPoint GetStartPoint(const Axis axis) const override;
    virtual EndPoint GetEndPoint(const Axis axis) const override;

    virtual void DrawDebug() override;

    float m_radius;

private:
    DebugCuboid* m_debugCube;

};

