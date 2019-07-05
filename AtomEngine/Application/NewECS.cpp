#include "NewECS.h"
#include "ThreadPool.h"
#include "ShaderManager.h"
#include "ScreenManager.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "UniformBufferTypes.h"
#include "GUIManager.h"
#include "MeshComponent.h"
#include "TransformComponent.h"
#include "RenderMeshSystem.h"
#include "NumberGenerator.h"

#include <GLM/gtx/rotate_vector.hpp>
#include "RigidBodyComponent.h"
#include "BoundingVolumeHeirarchy.h"


NewECS::NewECS() :
m_renderMeshSystem(m_instanceRenderer),
m_renderDebugSystem(m_debugRenderer)
{
    JobSystem::Instance();
}


NewECS::~NewECS()
{
}

void NewECS::CreateSimulation()
{
    TransformComponent transform;
    transform.m_transform.SetPosition(glm::vec3(0, 0, 5));
    transform.m_transform.SetScale(glm::vec3(1.0f));
    MeshComponent meshComp;
    meshComp.m_mesh.LoadMesh("Assets/Models/cube.obj");
    RigidBodyComponent* rigidbody;
    AABBComponent* aabb;

    NumberGenerator genny;
    genny.SetSeed(0);
    genny.SetRange(-20, 20);

    //Create Entities;
    for (int x = -5; x < 5; x++) {
        for (int y = -5; y < 5; y++) {
            for (int z = -5; z < 5; z++) {
                transform.m_transform.SetPosition(glm::vec3(x, y , z ));
                transform.m_transform.SetScale(glm::vec3(std::abs(genny.GetNumberF()) + 0.1f, std::abs(genny.GetNumberF()) + 0.1f, std::abs(genny.GetNumberF()) + 0.1f) * 0.1f);

                auto entity = m_ecs.MakeEntity();
                m_ecs.AddComponent(entity, &transform);
                m_ecs.AddComponent(entity, &meshComp);
                rigidbody = new RigidBodyComponent(m_ecs.GetComponent<TransformComponent>(entity)->m_transform);
                rigidbody->m_rigidBody.SetMass(3.0f);
                //rigidbody->m_rigidBody.UseGravity();

                rigidbody->m_rigidBody.ApplyTorque(glm::vec3(0.01f, 0.01f, 0.01f));
                rigidbody->m_rigidBody.ApplyForce(glm::vec3(genny.GetNumberF(), genny.GetNumberF(), genny.GetNumberF()) * 0.1f);

                m_ecs.AddComponent(entity, rigidbody);

                aabb = new AABBComponent(&m_ecs.GetComponent<MeshComponent>(entity)->m_mesh);
                m_ecs.AddComponent(entity, aabb);
            }
        }
    }
}

bool NewECS::Initialize()
{
    camTransform = new POD_Transform();
    camTransform->SetPosition(glm::vec3(0, 0, -20));
    
    Input::Instance()->SetGUICallback([](SDL_Event* evt) { GUI::Instance()->ProcessInput(evt); });

    m_debugRenderer.Initialize("DEBUG", "debugInstance");
    m_skyboxRenderer.Initialize("SKYBOX", "skybox");
    m_skyboxRenderer.SetSkybox("OM_skybox/OM.jpg");
    m_instanceRenderer.Initialize("INSTANCE", "phongInstance");
    m_mainRenderer.Initialize("POST", "postProcessing");


    
    Shaders::Instance()->UseShader("INSTANCE");
    Shaders::Instance()->GetCurrentShader()->SetVec3("lightPos", glm::vec3(1.2f, 1.0f, 2.0f));
    Shaders::Instance()->GetCurrentShader()->SetVec3("viewPos", glm::vec3(0,0,0));
    Shaders::Instance()->GetCurrentShader()->SetVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
    Shaders::Instance()->GetCurrentShader()->SetVec3("lightColor", glm::vec3(1.0f));
    

    //Testing new ECS
    //Create A Mesh Component
    CreateSimulation();


    //Create Systems
    m_renderPipeline.AddSystem(&m_renderMeshSystem);
    m_renderPipeline.AddSystem(&m_renderDebugSystem);

    m_physicsSystems.AddSystem(&m_physicsMovementSystem);
    m_collisionDetection.SetBroadPhase<BoundingVolumeHeirarchy>(&m_debugRenderer);
    m_collisionDetection.SetNarrowPhase<GJK>();
    m_physicsSystems.AddSystem(&m_collisionDetection);

    GUI::Instance()->SetRenderDebugSystem(&m_renderDebugSystem);
    GUI::Instance()->SetCollisionDetectionSystem(&m_collisionDetection);

    return true;
}

void NewECS::Input()
{
    //DRAWING INPUTS
    if (Input::Instance()->IsKeyPressed(SDLK_F1)) {
        m_wireFrame = !m_wireFrame;
    }
    if (Input::Instance()->IsKeyPressed(SDLK_F2))
    {
        m_showDebug = !m_showDebug;
    }
    //QUIT INPUT
    if (Input::Instance()->IsKeyPressed(SDLK_ESCAPE)) {
        Input::Instance()->RequestQuit();
    }


    //TESTING
    if (Input::Instance()->IsKeyPressed(SDLK_LALT)) {
        Input::Instance()->CaptureMouse(!Input::Instance()->IsMouseCaptured());
    }


    if (Input::Instance()->IsKeyHeld(SDLK_w)) {
        camTransform->Translate(camTransform->GetForward());
    }
    if (Input::Instance()->IsKeyHeld(SDLK_s)) {
        camTransform->Translate(-camTransform->GetForward());
    }
    if (Input::Instance()->IsKeyHeld(SDLK_a)) {
        camTransform->Translate(camTransform->GetRight());
    }
    if (Input::Instance()->IsKeyHeld(SDLK_d)) {
        camTransform->Translate(-camTransform->GetRight());
    }
    if (Input::Instance()->IsKeyHeld(SDLK_SPACE)) {
        camTransform->Translate(glm::vec3(0, 1, 0));
    }
    if (Input::Instance()->IsKeyHeld(SDLK_LSHIFT)) {
        camTransform->Translate(-glm::vec3(0, 1, 0));
    }

    if (Input::Instance()->HasMouseMoved() && Input::Instance()->IsMouseCaptured()) {

        glm::vec2 rot = glm::vec2(Input::Instance()->GetMouseMove().m_relativeX, Input::Instance()->GetMouseMove().m_relativeY);
        camTransform->Rotate(glm::vec3(rot.x, -rot.y, 0.0) * 0.1f);

        /*if (camera != nullptr)
        {
            if (camera->IsPitchConstrained()) {
                auto front = glm::cross(_transform->GetRight(), glm::vec3(0, 1, 0));
                auto pitch = glm::degrees(glm::angle(front, _transform->GetForward()));

                if (pitch > 85.0f)
                {
                    _transform->Rotate(glm::vec3(0, rot.y, 0));
                }
                else if (pitch < -85.0f)
                {
                    _transform->Rotate(glm::vec3(0, rot.y, 0));
                }
            }
        }*/

    }

}

void NewECS::Update(float delta)
{
    m_ecs.UpdateSystems(m_physicsSystems, delta);
}

void NewECS::Render()
{
    glm::mat4 view = glm::lookAt(camTransform->GetPosition(), camTransform->GetPosition() + camTransform->GetForward(), camTransform->GetUp());

    m_mainRenderer.SetViewMatrix(view);
    m_mainRenderer.BeginFrame();

    if (m_wireFrame) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    m_ecs.UpdateSystems(m_renderPipeline, 0);
    m_instanceRenderer.Render();
    m_debugRenderer.Render();
    m_skyboxRenderer.Render();
    
    
    if (m_wireFrame) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    m_mainRenderer.Render();

    GUI::Instance()->ShowGUI();
}

bool NewECS::Shutdown()
{
    return true;
}

void NewECS::Pause()
{
}

void NewECS::Resume()
{
}

bool NewECS::IsPaused()
{
    return m_isPaused;
}
