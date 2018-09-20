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
#include "GLM/gtc/type_ptr.hpp"
#include "Buffer.h"


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

    while (!IsJobReady(test)) {
        Logger::Instance()->LogDebug("Job Not Done!!");
    }*/

    Shaders::Instance()->AddShader("PHONG", "phong");
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

    Screen::Instance()->Enable3D();

    struct testBuffer {
        glm::mat4 projection;
        glm::mat4 view;
    } testBuffer;
    testBuffer = { Screen::Instance()->GetProjection(), camera->GetComponent<Camera>()->GetViewMatrix() };

    int ugh = sizeof(testBuffer);
    
    Shaders::Instance()->GetShader("PHONG")->BindUniformBuffer("Matrices", UniformBufferBinding::MATRICES);
    
    testUni.Create(sizeof(testBuffer));
    testUni.BindBuffer(UniformBufferBinding::MATRICES);
    testUni.SetData(&testBuffer, sizeof(testBuffer));
    //glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(testBuffer.projection));
    //glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(testBuffer.view));


    return true;
}

void LoadState::Input()
{
    if (Input::Instance()->IsKeyPressed(SDLK_w)) {
        _wireframe = true;
    }
    if (Input::Instance()->IsKeyPressed(SDLK_s)) {
        _wireframe = false;
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
    

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, rotation.y, glm::vec3(0, 1, 0));

    glm::mat4 view = GameObjectList.front()->GetComponent<Camera>()->GetViewMatrix();

    frameBuffer.Bind();
    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);
    Screen::Instance()->Clear();

    if (_wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    Shaders::Instance()->UseShader("PHONG");
    Shaders::Instance()->GetCurrentShader()->SetMat4("model", model);
    //Shaders::Instance()->GetCurrentShader()->UpdateMatrices(model, view, Screen::Instance()->GetProjection());

    //Shaders::Instance()->GetCurrentShader()->SetVec3("aColor", glm::vec3(0.1, 0.5f, 1));

    Shaders::Instance()->GetCurrentShader()->SetVec3("lightPos", glm::vec3(1.2f, 1.0f, 2.0f));
    Shaders::Instance()->GetCurrentShader()->SetVec3("viewPos", GameObjectList.front()->GetComponent<Transform>()->GetPosition());
    Shaders::Instance()->GetCurrentShader()->SetVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
    Shaders::Instance()->GetCurrentShader()->SetVec3("lightColor", glm::vec3(1.0f));

    for (auto obj : GameObjectList) {
        Mesh* mesh = obj->GetComponent<Mesh>();
        if (mesh != nullptr) {
            mesh->Render();
        }
    }

    if (_wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    frameBuffer.Unbind();
    glViewport(0, 0, 800, 600);
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);

    Shaders::Instance()->UseShader("FRAMETEST");
    Shaders::Instance()->GetShader("FRAMETEST")->SetInt("myTexture", 0);

    glActiveTexture(GL_TEXTURE0);
    frameBuffer.BindTexture(0);
    quadTest->Render();
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
