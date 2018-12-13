#include "LoadState.h"
#include "ThreadPool.h"
#include "LogManager.h"
#include "InputManager.h"

#include "GameObject.h"
#include "ShaderManager.h"
#include "ScreenManager.h"
//#include "IOManager.h"

#include "Buffer.h"
#include "Kernel.h"

//#include "SerialRegister.h"
//#include "SerialExtensions.h"
#include "CameraControls.h"
#include "GUIManager.h"

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

   /*auto test = JobSystem::Instance()->AddJob([] {
        Logger::Instance()->LogDebug("Single Test");
    });

    while (!IsJobReady(test)) {
        Logger::Instance()->LogDebug("Job Not Done!!");
    }*/

    Shaders::Instance()->AddShader("PHONG", "phong");
    Shaders::Instance()->AddShader("BASIC", "basic");
    Shaders::Instance()->AddShader("INSTANCE", "phongInstance");
    Shaders::Instance()->AddShader("SKYBOX", "skybox");
    Shaders::Instance()->AddShader("POST", "postProcessing");

    /*IO::Instance()->Open("GameScene.xml", std::ios::in);
    IO::Instance()->Serialize<cereal::XMLInputArchive>(m_scene);
    IO::Instance()->Close();*/

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

    return true;
}

void LoadState::Input()
{
    //DRAWING INPUTS
    if (Input::Instance()->IsKeyPressed(SDLK_F1)) {
        m_wireFrame = !m_wireFrame;
    }
    //QUIT INPUT
    if (Input::Instance()->IsKeyPressed(SDLK_ESCAPE)) {
        Input::Instance()->RequestQuit();
    }
}

void LoadState::Update(float delta)
{
    GUI::Instance()->SetSceneData(&m_scene);
    m_gameObjects = m_scene->GetGameObjects();
    for (auto& obj : m_gameObjects) {
        obj->Update(delta);
    }
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

    //ImGui::ShowDemoWindow();

    GUI::Instance()->ShowGUI();
}

bool LoadState::Shutdown()
{
    /*for (auto& obj : m_gameObjectList) {
        auto temp = obj.get();
        GameObject::Destroy(obj);
    }
    m_gameObjectList.clear();*/
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
