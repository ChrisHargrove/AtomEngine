#pragma once

#include "Component.h"
#include "Transform.h"

class CameraControls : public Component
{
public:
    CameraControls();
    virtual ~CameraControls();

    virtual void Update(float deltaTime) override;
    virtual bool Initialize() override;

    template<class Archive>
    void serialize(Archive &archive) {
    }

private:
    Transform* _transform;
    bool _mouseCaptured;
};

CEREAL_REGISTER_TYPE_WITH_NAME(CameraControls, "CameraCoontrols");
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, CameraControls);

