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
#include "GLM/gtc/matrix_transform.hpp"


LoadState::LoadState()
{
    JobSystem::Instance();
}


LoadState::~LoadState()
{
}

bool LoadState::Initialize()
{
    /*auto test = JobSystem::Instance()->AddJob([] {
        Logger::Instance()->LogDebug("Single Test");
    });

    if (IsJobReady(test)) {
        Logger::Instance()->LogDebug("Job Completed!");
    }*/

    Shaders::Instance()->AddShader("BASIC", "basic");
    Shaders::Instance()->AddShader("FRAMETEST", "frameBufferTest");

    GameObject* camera = new GameObject();
    camera->AddComponent<Camera>();

    //LOADING MODELS IS SUCCESSFUL
    GameObject* meshTest = new GameObject();
    Mesh* mesh = new Mesh();
    mesh->SetMesh("Assets/Models/cactus_one.obj");
    meshTest->AddComponent(mesh);

    //TESTING QUAD
    quadTest = new Quad(true);

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

    

    return true;
}

void LoadState::Input()
{
    if (Input::Instance()->IsKeyPressed(SDLK_w)) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if (Input::Instance()->IsKeyPressed(SDLK_s)) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
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
    Screen::Instance()->Enable3D();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, rotation.y, glm::vec3(0, 1, 0));

    glm::mat4 view = GameObjectList.front()->GetComponent<Camera>()->GetViewMatrix();

    frameBuffer.Bind();
    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);
    Screen::Instance()->Clear();

    Shaders::Instance()->UseShader("BASIC");

    Shaders::Instance()->GetCurrentShader()->UpdateMatrices(model, view, Screen::Instance()->GetProjection());

    for (auto obj : GameObjectList) {
        Mesh* mesh = obj->GetComponent<Mesh>();
        if (mesh != nullptr) {
            mesh->Render();
        }
    }


    frameBuffer.Unbind();
    glViewport(0, 0, 800, 600);
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);

    Shaders::Instance()->UseShader("FRAMETEST");
    //model = glm::mat4(1.0f);
    //Shaders::Instance()->GetCurrentShader()->UpdateMatrices(model, view, Screen::Instance()->GetProjection());
    Shaders::Instance()->GetShader("FRAMETEST")->SetInt("myTexture", 0);
    glActiveTexture(GL_TEXTURE0);
    //frameBuffer.BindTexture();
    quadTest->Render(frameBuffer.GetTexture());

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
