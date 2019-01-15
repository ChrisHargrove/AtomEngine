#pragma once

#include "Component.h"
#include "DebugCuboid.h"
#include "Collider.h"
#include "PhysicsTypes.h"

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

class ATOM_API BoxCollider : public Collider
{
public:
    friend class PhysicsManager;

    BoxCollider();
    virtual ~BoxCollider();

    bool Initialize() override;
    void Update(float deltaTime) override;

protected:

    virtual EndPoint GetStartPoint(const Axis axis) const override;
    virtual EndPoint GetEndPoint(const Axis axis) const override;

    virtual void DrawDebug() override;

private:

    //OBB
    std::vector<glm::vec3> RecalculateOBB();
    glm::vec3 m_verticesOBB[8];

    //AABB
    void RecalculateAABB();

    //Debug Drawing
    DebugCuboid* m_debugCubeAABB;
    DebugCuboid* m_debugCubeOBB;

};

