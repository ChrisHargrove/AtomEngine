#pragma once

#include "Component.h"
#include "DebugCuboid.h"
#include "Collider.h"
#include "PhysicsTypes.h"
#include <array>

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

    virtual void DrawDebug() override;

    friend class cereal::access;

    template <class Archive>
    void serialize(Archive &archive) {
    }

private:

    //optimizing
    void RecalculateOBB(std::array<glm::vec3, 8>& vertices);

    //OBB
    std::vector<glm::vec3> RecalculateOBB();
    std::array<glm::vec3, 8> m_verticesOBB;

    //AABB
    void RecalculateAABB();

    //Debug Drawing
    DebugCuboid* m_debugCubeAABB;

};

CEREAL_REGISTER_TYPE_WITH_NAME(BoxCollider, "BoxColliderObject");

