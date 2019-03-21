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


NewECS::NewECS() :
m_screenFrame(nullptr),
m_renderMeshSystem(m_renderer)
{
    JobSystem::Instance();
}


NewECS::~NewECS()
{
}

bool NewECS::Initialize()
{
    camTransform = new POD_Transform();
    
    Input::Instance()->SetGUICallback([](SDL_Event* evt) { GUI::Instance()->ProcessInput(evt); });

    Shaders::Instance()->AddShader("INSTANCE", "phongInstance");
    Shaders::Instance()->AddShader("SKYBOX", "skybox");
    Shaders::Instance()->AddShader("POST", "postProcessing");

    m_screenFrame = new Quad(true);

    m_frameBuffer.Create(Screen::Instance()->GetSize());
    m_frameBuffer.AddAttachment(TEXTURE);
    m_frameBuffer.AddRenderBuffer(DEPTH_STENCIL);
    if (!m_frameBuffer.IsFrameBufferComplete()) {
        return false;
    }

    Screen::Instance()->Enable3D();

    MatriceBuffer m_matrixBuffer = { Screen::Instance()->GetProjection(), glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0)) };
    Shaders::Instance()->SetUniformBufferBinding("Matrices", UniformBufferBinding::MATRICES);
    m_uniformBuffer.Create(sizeof(m_matrixBuffer));
    m_uniformBuffer.BindBuffer(UniformBufferBinding::MATRICES);
    m_uniformBuffer.SetSubData(&MatriceBuffer::m_projection, &m_matrixBuffer.m_projection);
    m_uniformBuffer.SetSubData(&MatriceBuffer::m_view, &m_matrixBuffer.m_view);

    Shaders::Instance()->UseShader("SKYBOX");
    Shaders::Instance()->GetCurrentShader()->SetMat4("model", glm::mat4(1.0f));
    Shaders::Instance()->GetCurrentShader()->SetInt("skybox", 0);

    m_skybox = std::make_shared<Skybox>();
    m_skybox->Load("OM_skybox/OM.jpg");

    Shaders::Instance()->UseShader("INSTANCE");

    Shaders::Instance()->GetCurrentShader()->SetVec3("lightPos", glm::vec3(1.2f, 1.0f, 2.0f));
    Shaders::Instance()->GetCurrentShader()->SetVec3("viewPos", glm::vec3(0,0,0));
    Shaders::Instance()->GetCurrentShader()->SetVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
    Shaders::Instance()->GetCurrentShader()->SetVec3("lightColor", glm::vec3(1.0f));

    Shaders::Instance()->UseShader("POST");
    Shaders::Instance()->GetShader("POST")->SetInt("myTexture", 0);

    //Testing new ECS
    //Create A Mesh Component
    TransformComponent transform;
    transform.m_transform.SetPosition(glm::vec3(0, 0, -20));
    MeshComponent meshComp;
    meshComp.m_mesh.LoadMesh("Assets/Models/cube.obj");
    //Create Entities;
    auto entity = m_ecs.MakeEntity();
    m_ecs.AddComponent(entity, &transform);
    m_ecs.AddComponent(entity, &meshComp);

    NumberGenerator genny;
    genny.SetSeed(0);
    genny.SetRange(-50, 50);

    for(int i = 0; i < 10000; i++)
    {
        transform.m_transform.SetPosition(glm::vec3(genny.GetNumberF(), genny.GetNumberF(), genny.GetNumberF()));

        auto entity = m_ecs.MakeEntity();
        m_ecs.AddComponent(entity, &transform);
        m_ecs.AddComponent(entity, &meshComp);
    }

    //Create Systems
    m_renderPipeline.AddSystem(&m_renderMeshSystem);

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
    viewFloat += 0.001f;
}

void NewECS::Render()
{
    glm::vec3 direction(0, 0, 1);
    direction = glm::normalize(glm::rotate(direction, viewFloat, glm::vec3(0, 1, 0)));

    view = glm::lookAt(camTransform->GetPosition(), camTransform->GetPosition() + camTransform->GetForward(), camTransform->GetUp());

    //m_renderer.GetUniformBuffer().SetSubData(&MatriceBuffer::m_view, &view);
    m_uniformBuffer.SetSubData(&MatriceBuffer::m_view, &view);

    m_frameBuffer.Bind();
    Screen::Instance()->EnableDepthTesting(true);
    Screen::Instance()->Clear();

    if (m_wireFrame) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    Shaders::Instance()->UseShader("INSTANCE");
    m_ecs.UpdateSystems(m_renderPipeline, 0);
    m_renderer.Render();

    Shaders::Instance()->UseShader("SKYBOX");
    m_skybox->Render();


    if (m_wireFrame) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    m_frameBuffer.Unbind();
    Screen::Instance()->EnableDepthTesting(false);
    Screen::Instance()->Clear(ClearBits::COLOR);

    Shaders::Instance()->UseShader("POST");

    glActiveTexture(GL_TEXTURE0);
    m_frameBuffer.BindTexture(0);
    m_screenFrame->Render();
    m_frameBuffer.BindTexture();

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
