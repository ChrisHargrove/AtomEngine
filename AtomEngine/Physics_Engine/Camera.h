#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include "Component.h"
#include <GLM/glm.hpp>

class ATOM_API Camera : public Component
{
public:
    Camera();
    virtual ~Camera();

    float GetZoom();
    glm::mat4 GetViewMatrix();
    glm::mat4 GetOrthoMatrix();

    void EnablePitchConstraint(bool enable = true);
    void SetTarget(glm::vec3 target);

private:

    virtual void Initialize() override;
    virtual void Update(float deltaTime) override;

    float m_zoom;
    bool m_isPitchConstrained;

    glm::vec3 m_target;
    glm::mat4 m_viewMatrix;
};

