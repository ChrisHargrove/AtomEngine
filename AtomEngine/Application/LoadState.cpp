#include "LoadState.h"
#include "ThreadPool.h"
#include "LogManager.h"
#include "InputManager.h"

#include "GameObject.h"
#include "ShaderManager.h"
#include "ScreenManager.h"

#include "Buffer.h"
#include "Kernel.h"

#include "CameraControls.h"
#include "GUIManager.h"
#include "ResourceManager.h"
#include "PhysicsManager.h"
#include "BoxCollider.h"
#include "NumberGenerator.h"

LoadState::LoadState(): 
m_screenFrame(nullptr)
{
    JobSystem::Instance();
}

LoadState::~LoadState()
{
    m_scene.reset();
}

bool LoadState::Initialize()
{
    Input::Instance()->SetGUICallback([](SDL_Event* evt) {GUI::Instance()->ProcessInput(evt); });

    Shaders::Instance()->AddShader("PHONG", "phong");
    Shaders::Instance()->AddShader("BASIC", "basic");
    Shaders::Instance()->AddShader("INSTANCE", "phongInstance");
    Shaders::Instance()->AddShader("SKYBOX", "skybox");
    Shaders::Instance()->AddShader("POST", "postProcessing");

    if(m_scene == nullptr)
    {
        m_scene = std::make_shared<Scene>();
    }

    m_screenFrame = new Quad(true);

    m_frameBuffer.Create(Screen::Instance()->GetSize());
    m_frameBuffer.AddAttachment(TEXTURE);
    m_frameBuffer.AddRenderBuffer(DEPTH_STENCIL);
    if (!m_frameBuffer.IsFrameBufferComplete()) {
        return false;
    }

    Screen::Instance()->Enable3D();

    Shaders::Instance()->GetShader("POST")->SetKernel(Kernel::Identity);

    m_scene->Initialize();

    m_gameObjects = m_scene->GetGameObjects();

    m_scene->GetSceneCamera()->GetParent()->AddComponent<CameraControls>();

    std::shared_ptr<Skybox> test = std::make_shared<Skybox>();
    test->Load("OM_skybox/OM.jpg");

    Resource::Instance()->AddResource(test.get()->GetName(), test);

    m_scene->SetSkybox(test.get()->GetName());

    GUI::Instance()->SetSceneData(&m_scene);

    Physics::Instance()->Initialize();

    return true;
}

void LoadState::Input()
{
    //DRAWING INPUTS
    if (Input::Instance()->IsKeyPressed(SDLK_F1)) {
        m_wireFrame = !m_wireFrame;
    }
    if(Input::Instance()->IsKeyPressed(SDLK_F2))
    {
        m_showDebug = !m_showDebug;
    }
    //QUIT INPUT
    if (Input::Instance()->IsKeyPressed(SDLK_ESCAPE)) {
        Input::Instance()->RequestQuit();
    }

    //TESTING
    if(Input::Instance()->IsKeyPressed(SDLK_F8))
    {
        Physics::Instance()->Initialize();
    }
    if(Input::Instance()->IsKeyPressed(SDLK_F10))
    {
        NumberGenerator random;
        random.SetRange(-1, 1);

        auto count = 0;
        for(int x = 0; x < 10; x++)
        {
            for (int y = 0; y < 10; y++)
            {
                for (int z = 0; z < 10; z++)
                {
                    count++;
                    auto obj = std::make_shared<GameObject>();
                    obj->AddComponent<Transform>();
                    auto mesh = std::make_shared<Mesh>();
                    mesh->SetMesh("Assets/Models/cube.obj");
                    obj->AddComponent(mesh);
                    auto rbody = std::make_shared<RigidBody>();
                    rbody->UsesGravity() = true;

                   
                    obj->AddComponent(rbody);
                    Physics::Instance()->AddBody(rbody.get());
                    obj->AddComponent<BoxCollider>();

                    obj->SetName(std::to_string(count));
                    m_scene->AddGameObject(obj);

                    rbody->ApplyTorque(glm::vec3(random.GetNumberF(), random.GetNumberF(), random.GetNumberF()) * 100.0f);
                    rbody->ApplyForce(glm::vec3(random.GetNumberF(), random.GetNumberF(), random.GetNumberF()));


                    obj->GetComponent<Transform>()->Translate(glm::vec3(x,y,z) * 4.0f);

                    m_scene->AddMesh(mesh.get());
                    

                }
            }
        }
    }
}

void LoadState::Update(float delta)
{
    m_gameObjects = m_scene->GetGameObjects();

    Physics::Instance()->StepSimulation(delta);

    m_scene->Update(delta);
}

void LoadState::Render()
{
    glm::mat4 view = m_scene->GetSceneCamera()->GetViewMatrix();

    m_frameBuffer.Bind();
    Screen::Instance()->CreateViewport();
    Screen::Instance()->EnableDepthTesting(true);
    Screen::Instance()->Clear();

    if (m_wireFrame) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    

    Shaders::Instance()->UseShader("INSTANCE");

    Shaders::Instance()->GetCurrentShader()->SetVec3("lightPos", glm::vec3(1.2f, 1.0f, 2.0f));
    Shaders::Instance()->GetCurrentShader()->SetVec3("viewPos", m_scene->GetSceneCamera()->GetComponent<Transform>()->GetPosition());
    Shaders::Instance()->GetCurrentShader()->SetVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
    Shaders::Instance()->GetCurrentShader()->SetVec3("lightColor", glm::vec3(1.0f));

    m_scene->Render();

    //Draw all physics debug stuff
    Shaders::Instance()->UseShader("BASIC");
    Physics::Instance()->DrawDebug();


    if (m_wireFrame) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    m_frameBuffer.Unbind();
    Screen::Instance()->CreateViewport();
    Screen::Instance()->EnableDepthTesting(false);
    Screen::Instance()->Clear(ClearBits::COLOR);

    Shaders::Instance()->UseShader("POST");
    Shaders::Instance()->GetShader("POST")->SetInt("myTexture", 0);

    glActiveTexture(GL_TEXTURE0);
    m_frameBuffer.BindTexture(0);
    m_screenFrame->Render();
    m_frameBuffer.BindTexture();

    ImGui::ShowDemoWindow();

    GUI::Instance()->ShowGUI();
}

bool LoadState::Shutdown()
{
    return true;
}

void LoadState::Pause()
{
}

void LoadState::Resume()
{
}

bool LoadState::IsPaused()
{
    return false;
}
