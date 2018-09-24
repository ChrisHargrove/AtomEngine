#pragma once

#include "Component.h"

class CameraControls : public Component
{
public:
    CameraControls();
    ~CameraControls();

    virtual void Update(float deltaTime) override;
    virtual void Initialize() override;
};

