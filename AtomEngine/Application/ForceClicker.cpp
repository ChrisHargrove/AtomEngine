#include "ForceClicker.h"
#include "InputManager.h"
#include "RigidBody.h"


ForceClicker::ForceClicker()
{
    m_name = "ForceClicker";
    m_typeInfo = &typeid(this);
}


ForceClicker::~ForceClicker()
{
}

void ForceClicker::Update(float deltaTime)
{
    if(Input::Instance()->IsMouseCaptured())
    {
        if (Input::Instance()->IsButtonClicked(SDL_BUTTON_LEFT))
        {
            GetComponent<RigidBody>()->ApplyForce(glm::vec3(0, 0, 0.2), glm::vec3(0,1, -1));
        }
        if(Input::Instance()->IsButtonClicked(SDL_BUTTON_RIGHT))
        {
            GetComponent<RigidBody>()->ApplyForce(glm::vec3(1, 0, 0));
        }
        if(Input::Instance()->IsKeyPressed(SDLK_LEFT))
        {
            GetComponent<RigidBody>()->ApplyTorque(glm::vec3(0, 0, 1));
        }
        if (Input::Instance()->IsKeyPressed(SDLK_RIGHT))
        {
            GetComponent<RigidBody>()->ApplyTorque(glm::vec3(0, 0, -1));
        }
    }
}

bool ForceClicker::Initialize()
{
    return true;
}
