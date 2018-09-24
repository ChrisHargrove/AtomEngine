#include "LoadState.h"
#include "ThreadPool.h"
#include "LogManager.h"
#include "InputManager.h"

#include "GameObject.h"
#include "Camera.h"
#include "Transform.h"
#include "ShaderManager.h"
#include "ScreenManager.h"

#include "Cuboid.h"
#include "Mesh.h"
#include "Buffer.h"
#include "Utilities.h"
#include "Kernel.h"


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

    GameObject* camera = new GameObject();
    m_mainCamera = new Camera();
    camera->AddComponent(m_mainCamera);

    //LOADING MODELS IS SUCCESSFUL
    GameObject* meshTest = new GameObject();
    Mesh* mesh = new Mesh();
    mesh->SetMesh("Assets/Models/cactus_one.obj");
    meshTest->AddComponent(mesh);

    //TESTING QUAD
    screenFrame = new Quad(true);

    GameObjectList.push_back(camera);
    GameObjectList.push_back(meshTest);

    for (GameObject* obj : GameObjectList) {
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
    for (GameObject* obj : GameObjectList) {
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

    for (auto obj : GameObjectList) {
        Mesh* mesh = obj->GetComponent<Mesh>();
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
    for (auto obj : GameObjectList) {
        delete obj;
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
