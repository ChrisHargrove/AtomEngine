#pragma once

#include "Component.h"

class CameraControls : public Component
{
public:
    CameraControls();
    virtual ~CameraControls();

    virtual void Update(float deltaTime) override;
    virtual void Initialize() override;

    template<class Archive>
    void serialize(Archive &archive) {
    }

private:
    Component* _transform;
};

CEREAL_REGISTER_TYPE_WITH_NAME(CameraControls, "CameraCoontrols");
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, CameraControls);

