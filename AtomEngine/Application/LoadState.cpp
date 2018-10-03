#include "LoadState.h"
#include "ThreadPool.h"
#include "LogManager.h"
#include "InputManager.h"

#include "GameObject.h"
#include "Camera.h"
#include "CameraControls.h"
#include "Transform.h"
#include "ShaderManager.h"
#include "ScreenManager.h"
#include "IOManager.h"

#include "Cuboid.h"
#include "Mesh.h"
#include "Buffer.h"
#include "Utilities.h"
#include "Kernel.h"

#include <CEREAL/cereal.hpp>

#include "SerialRegister.h"
#include "SerialExtensions.h"


LoadState::LoadState()
{
    JobSystem::Instance();
}


LoadState::~LoadState()
{
}

struct MatriceBuffer {
    glm::mat4 projection;
    glm::mat4 view;
};

bool LoadState::Initialize()
{

   /*auto test = JobSystem::Instance()->AddJob([] {
        Logger::Instance()->LogDebug("Single Test");
    });

    while (!IsJobReady(test)) {
        Logger::Instance()->LogDebug("Job Not Done!!");
    }*/

    Shaders::Instance()->AddShader("PHONG", "phong");
    Shaders::Instance()->AddShader("BASIC", "basic");
    Shaders::Instance()->AddShader("FRAMETEST", "postProcessing");

    std::shared_ptr<GameObject> camera = std::make_shared<GameObject>();
    m_mainCamera = std::make_shared<Camera>();
    camera->AddComponent<Transform>();
    camera->AddComponent(m_mainCamera);
    camera->AddComponent<CameraControls>();


    //LOADING MODELS IS SUCCESSFUL
    std::shared_ptr<GameObject> meshTest = std::make_shared<GameObject>();
    meshTest->AddComponent<Transform>();
    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
    mesh->SetMesh("Assets/Models/cactus_one.obj");
    meshTest->AddComponent(mesh);

    //TESTING QUAD
    screenFrame = new Quad(true);

    GameObjectList.push_back(camera);
    GameObjectList.push_back(meshTest);

    for (auto obj : GameObjectList) {
        obj->Initialize();
    }

    rotation = glm::vec3(0, 0, 0);

    camera->GetComponent<Transform>()->SetPosition(glm::vec3(0, 0, -4));

    frameBuffer.Create(Screen::Instance()->GetSize());
    frameBuffer.AddAttachment(TEXTURE);
    frameBuffer.AddRenderBuffer(DEPTH_STENCIL);
    if (!frameBuffer.IsFrameBufferComplete()) {
        return false;
    }

    Screen::Instance()->Enable3D();

    MatriceBuffer testBuffer = { Screen::Instance()->GetProjection(), m_mainCamera->GetViewMatrix() };

    Shaders::Instance()->GetShader("PHONG")->BindUniformBuffer("Matrices", UniformBufferBinding::MATRICES);
    Shaders::Instance()->GetShader("FRAMETEST")->SetKernel(Kernel::Identity);
    
    testUni.Create(sizeof(testBuffer));
    testUni.BindBuffer(UniformBufferBinding::MATRICES);
    testUni.SetSubData(&MatriceBuffer::projection, &testBuffer.projection);
    testUni.SetSubData(&MatriceBuffer::view, &testBuffer.view);

    //Screen::Instance()->CaptureMouse();

    //{
    //    std::ofstream output;
    //    output.open("cereal.test.xml");
    //    cereal::XMLOutputArchive archive(output);

    //    //TODO: Test if serializign individual objects works better!!! bugs when using the whole
    //    //vector currrently :/
    //    //28/9/18

    //    for (int i = 0; i < GameObjectList.size(); i++) {
    //        archive(CEREAL_NVP(GameObjectList[i]));
    //    }
    //}

    IO::Instance()->Open("cereal.test.xml", std::ios::out);
    IO::Instance()->Serialize<cereal::XMLOutputArchive>(GameObjectList[0]);
    IO::Instance()->Close();

    std::shared_ptr<GameObject> test = std::make_shared<GameObject>();

    IO::Instance()->Open("cereal.test.xml", std::ios::in);
    IO::Instance()->Serialize<cereal::XMLInputArchive>(test);
    IO::Instance()->Close();
       
    return true;
}

void LoadState::Input()
{
    //DRAWING INPUTS
    if (Input::Instance()->IsKeyPressed(SDLK_F1)) {
        _wireframe = !_wireframe;
    }    
    //QUIT INPUT
    if (Input::Instance()->IsKeyPressed(SDLK_ESCAPE)) {
        Input::Instance()->RequestQuit();
    }
}

void LoadState::Update(float delta)
{
    for (auto obj : GameObjectList) {
        obj->Update(delta);

    }
    rotation.y += delta;
}

void LoadState::Render()
{

    glm::mat4 view = GameObjectList.front()->GetComponent<Camera>()->GetViewMatrix();
    testUni.SetSubData(&MatriceBuffer::view, &view);

    frameBuffer.Bind();
    Screen::Instance()->CreateViewport();
    Screen::Instance()->EnableDepthTesting(true);
    Screen::Instance()->Clear();

    if (_wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    Shaders::Instance()->UseShader("PHONG");

    Shaders::Instance()->GetCurrentShader()->SetVec3("lightPos", glm::vec3(1.2f, 1.0f, 2.0f));
    Shaders::Instance()->GetCurrentShader()->SetVec3("viewPos", GameObjectList.front()->GetComponent<Transform>()->GetPosition());
    Shaders::Instance()->GetCurrentShader()->SetVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
    Shaders::Instance()->GetCurrentShader()->SetVec3("lightColor", glm::vec3(1.0f));

    for (std::shared_ptr<GameObject> obj : GameObjectList) {
        Mesh* mesh = obj.get()->GetComponent<Mesh>();
        if (mesh != nullptr) {
            Shaders::Instance()->GetCurrentShader()->SetMat4("model", mesh->GetComponent<Transform>()->GetTransform());
            mesh->GetComponent<Transform>()->Rotate(glm::vec3(1 * 0.01f, 0, 0));
            mesh->Render();
        }
    }

    if (_wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    frameBuffer.Unbind();
    Screen::Instance()->CreateViewport();
    Screen::Instance()->EnableDepthTesting(false);
    Screen::Instance()->Clear(ClearBits::COLOR);

    Shaders::Instance()->UseShader("FRAMETEST");
    Shaders::Instance()->GetShader("FRAMETEST")->SetInt("myTexture", 0);

    glActiveTexture(GL_TEXTURE0);
    frameBuffer.BindTexture(0);
    screenFrame->Render();
    frameBuffer.BindTexture();

}

bool LoadState::Shutdown()
{
    GameObject* temp;

    for (int i = 0; i < GameObjectList.size(); i++) {
        temp = GameObjectList[i].get();
        GameObject::Destroy(GameObjectList[i]);
    }
    GameObjectList.clear();
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
