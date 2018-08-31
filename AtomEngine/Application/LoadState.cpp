#include "LoadState.h"
#include "ThreadPool.h"
#include "LogManager.h"

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

    GameObject* camera = new GameObject();
    camera->AddComponent<Camera>();

    //LOADING MODELS IS SUCCESSFUL
    GameObject* meshTest = new GameObject();
    Mesh* mesh = new Mesh();
    mesh->SetMesh("Assets/Models/cactus_one.obj");
    meshTest->AddComponent(mesh);

    GameObjectList.push_back(camera);
    GameObjectList.push_back(meshTest);

    for (GameObject* obj : GameObjectList) {
        obj->Initialize();
    }

    rotation = glm::vec3(0, 0, 0);

    camera->GetComponent<Transform>()->SetPosition(glm::vec3(0, 0, -4));

    return true;
}

void LoadState::Input()
{
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

    Shaders::Instance()->UseShader("BASIC");
    Shaders::Instance()->GetShader("BASIC")->UpdateMatrices(model, view, Screen::Instance()->GetProjection());

    for (auto obj : GameObjectList) {
        Mesh* mesh = obj->GetComponent<Mesh>();
        if (mesh != nullptr) {
            mesh->Render();
        }
    }

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
