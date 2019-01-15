#pragma once

#include <Component.h>

class ForceClicker : public Component
{
public:
    ForceClicker();
    virtual ~ForceClicker();

    void Update(float deltaTime) override;
    bool Initialize() override;

    template<class Archive>
    void serialize(Archive &archive) {
    }

};

CEREAL_REGISTER_TYPE_WITH_NAME(ForceClicker, "ForceClicker");
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ForceClicker);