#include "Scene.h"
#include "Transform.h"
#include "UniformBufferTypes.h"

#include "ScreenManager.h"
#include "ShaderManager.h"

Scene::Scene(): 
m_mainCamera(nullptr),
m_name("Scene")
{
}


Scene::~Scene()
{
}

void Scene::Initialize()
{
    //LOAD SKYBOX
    if(m_skybox == nullptr) {
        m_skybox = std::make_shared<Skybox>();
        m_skybox->Load("OM_skybox/OM.jpg");
    }

    //LOAD A SCENE CAMERA
    std::shared_ptr<GameObject> sceneCam = std::make_shared<GameObject>();
    sceneCam->AddComponent<Transform>();
    std::shared_ptr<Camera> camera = std::make_shared<Camera>();
    sceneCam->AddComponent(camera);
    sceneCam->Initialize();

    m_sceneCamera = camera;


    auto testModel = std::make_shared<GameObject>();
    auto testMesh = std::make_shared<Mesh>();
    testMesh->SetMesh("Assets/Models/sphere.obj");
    testModel->AddComponent<Transform>();
    testModel->AddComponent(testMesh);
    m_gameObjectList.push_back(testModel);

    std::vector<Mesh*> meshList;

    //LOOP THROUGH ALL GAME OBJECTS
    for(auto& obj : m_gameObjectList) {
        //INITIALIZE EACH ONE
        obj->Initialize();
        //IF THE OBJECT HAS A CAMERA ATTACHED STORE A POINTER TO IT
        if(obj->GetComponent<Camera>() != nullptr) {
            m_mainCamera = obj->GetComponent<Camera>();
        }
        //IF THE OBJECT HAS A MESH STORE IT
        if(obj->GetComponent<Mesh>() != nullptr) {
            meshList.push_back(obj->GetComponent<Mesh>());
        }
    }
    //IF NO GAME CAMERA FOUND ASSIGN SCENE CAM TO IT
    if(m_mainCamera == nullptr) {
        m_gameObjectList.push_back(sceneCam);
        m_mainCamera = m_sceneCamera.get();
    }

    //LOOP THROUGH ALL MESHES
    std::string meshName = "";
    for(auto& mesh : meshList) {
        //IF A MESH IS BEING USED MORE THAN ONCE INCREMENT ITS COUNT
        //AND STORE ITS TRANSFORM IN A LIST OF TRANSFORMS
        if(meshName == mesh->GetMesh()) {
            m_renderList[meshName].second += 1;
            m_renderTranforms[meshName].push_back(mesh->GetComponent<Transform>()->GetTransform());
        }
        //IF ITS THE FIRST MESH OF THIS OF THIS NAME STORE IT AND IVE IT A COUNT OF 1
        //AND STORE ITS TRANSFORM TOO
        else {
            meshName = mesh->GetMesh();
            m_renderList.emplace(std::make_pair(meshName, std::make_pair(mesh, 1)));
            m_renderTranforms[meshName].push_back(mesh->GetComponent<Transform>()->GetTransform());
        }
    }

    //SET UP A COMMON MATRIX BUFFER FOR VIEW AND PROJECTION
    MatriceBuffer matrixBuffer = { Screen::Instance()->GetProjection(), m_sceneCamera->GetViewMatrix() };
    Shaders::Instance()->SetUniformBufferBinding("Matrices", UniformBufferBinding::MATRICES);

    m_matriceBuffer.Create(sizeof(matrixBuffer));
    m_matriceBuffer.BindBuffer(UniformBufferBinding::MATRICES);
    m_matriceBuffer.SetSubData(&MatriceBuffer::m_projection, &matrixBuffer.m_projection);
    m_matriceBuffer.SetSubData(&MatriceBuffer::m_view, &matrixBuffer.m_view);

    //LOOP THROUGH ALL RENDERABLE MESHES
    for(auto& mesh : m_renderList) {
        //CREATE A VERTEX BUFFER FOR THE MESH TRANSFORMS
        m_renderInstanceBuffers[mesh.first].Create(VBO);
        m_renderInstanceBuffers[mesh.first].Bind();
        //FILL THE BUFFER WITH THE MESHES TRANSFORMS
        std::vector<glm::mat4> transformList;
        transformList = m_renderTranforms[mesh.first];
        m_renderInstanceBuffers[mesh.first].FillBuffer(transformList.size() * sizeof(glm::mat4), &transformList[0], DYNAMIC);
        
        auto submeshList = mesh.second.first->GetSubMeshList();
        //LOOP THROUGH EACH SUBMESH AND BIND ITS VAO THEN CONFIGURE THE TRANSFORM BUFFER FOR INSTANCE
        //RENDERING
        for(auto& submesh : submeshList) {
            submesh->GetVAO()->Bind();
            m_renderInstanceBuffers[mesh.first].AddAttributePointer(BufferAttribute::INSTANCE_MAT_1, 4, VT_FLOAT, 4 * sizeof(glm::vec4));
            m_renderInstanceBuffers[mesh.first].AddAttributePointer(BufferAttribute::INSTANCE_MAT_2, 4, VT_FLOAT, 4 * sizeof(glm::vec4), sizeof(glm::vec4));
            m_renderInstanceBuffers[mesh.first].AddAttributePointer(BufferAttribute::INSTANCE_MAT_3, 4, VT_FLOAT, 4 * sizeof(glm::vec4), 2 * sizeof(glm::vec4));
            m_renderInstanceBuffers[mesh.first].AddAttributePointer(BufferAttribute::INSTANCE_MAT_4, 4, VT_FLOAT, 4 * sizeof(glm::vec4), 3 * sizeof(glm::vec4));

            m_renderInstanceBuffers[mesh.first].AddAtributeDivisor(BufferAttribute::INSTANCE_MAT_1, 1);
            m_renderInstanceBuffers[mesh.first].AddAtributeDivisor(BufferAttribute::INSTANCE_MAT_2, 1);
            m_renderInstanceBuffers[mesh.first].AddAtributeDivisor(BufferAttribute::INSTANCE_MAT_3, 1);
            m_renderInstanceBuffers[mesh.first].AddAtributeDivisor(BufferAttribute::INSTANCE_MAT_4, 1);
            submesh->GetVAO()->Unbind();
        }
        m_renderInstanceBuffers[mesh.first].Unbind();
    }
}

void Scene::Update()
{
    std::vector<Mesh*> meshes;
    

    for(auto& obj : m_gameObjectList)
    {
        auto mesh = obj->GetComponent<Mesh>();
        if(mesh != nullptr)
        {
            meshes.push_back(mesh);
            m_renderTranforms[mesh->GetMesh()].clear();
        }
    }
    for(auto& mesh : meshes)
    {
        m_renderTranforms[mesh->GetMesh()].push_back(mesh->GetComponent<Transform>()->GetTransform());
        auto transformList = m_renderTranforms[mesh->GetMesh()];
        m_renderInstanceBuffers[mesh->GetMesh()].FillBuffer(transformList.size() * sizeof(glm::mat4), &transformList[0], DYNAMIC);
    }
}

void Scene::Render()
{
    m_matriceBuffer.SetSubData(&MatriceBuffer::m_view, &m_sceneCamera->GetViewMatrix());

    Shaders::Instance()->UseShader("INSTANCE");
    for(auto& mesh : m_renderList) {
        auto instanceCount = mesh.second.second;
        mesh.second.first->Render(instanceCount);
    }

    Shaders::Instance()->UseShader("SKYBOX");
    Shaders::Instance()->GetCurrentShader()->SetMat4("model", glm::mat4(1.0f));
    m_skybox->Render();
}

Camera* Scene::GetMainCamera()
{
    if(m_mainCamera == nullptr)
    {
        return m_sceneCamera.get();
    }
    return m_mainCamera;
}

void Scene::SetMainCamera(Camera* camera)
{
    m_mainCamera = camera;
}

void Scene::SetName(const std::string& name)
{
    m_name = name;
}

std::string Scene::GetName()
{
    return m_name;
}

void Scene::AddGameObject()
{
    auto obj = std::make_shared<GameObject>();
    obj->AddComponent<Transform>();
    m_gameObjectList.push_back(obj);
}

void Scene::AddMesh(Mesh* mesh)
{
    auto renderList = m_renderList;

    if(renderList.empty())
    {
        m_renderList.emplace(std::make_pair(mesh->GetMesh(), std::make_pair(mesh, 1)));
        m_renderTranforms[mesh->GetMesh()].push_back(mesh->GetComponent<Transform>()->GetTransform());

        auto transformList = m_renderTranforms[mesh->GetMesh()];

        m_renderInstanceBuffers[mesh->GetMesh()].Create(VBO);
        m_renderInstanceBuffers[mesh->GetMesh()].Bind();
        m_renderInstanceBuffers[mesh->GetMesh()].FillBuffer(transformList.size() * sizeof(glm::mat4), &transformList[0], DYNAMIC);

        auto submeshList = mesh->GetSubMeshList();
        for (auto& submesh : submeshList) {
            submesh->GetVAO()->Bind();
            m_renderInstanceBuffers[mesh->GetMesh()].AddAttributePointer(BufferAttribute::INSTANCE_MAT_1, 4, VT_FLOAT, 4 * sizeof(glm::vec4));
            m_renderInstanceBuffers[mesh->GetMesh()].AddAttributePointer(BufferAttribute::INSTANCE_MAT_2, 4, VT_FLOAT, 4 * sizeof(glm::vec4), sizeof(glm::vec4));
            m_renderInstanceBuffers[mesh->GetMesh()].AddAttributePointer(BufferAttribute::INSTANCE_MAT_3, 4, VT_FLOAT, 4 * sizeof(glm::vec4), 2 * sizeof(glm::vec4));
            m_renderInstanceBuffers[mesh->GetMesh()].AddAttributePointer(BufferAttribute::INSTANCE_MAT_4, 4, VT_FLOAT, 4 * sizeof(glm::vec4), 3 * sizeof(glm::vec4));

            m_renderInstanceBuffers[mesh->GetMesh()].AddAtributeDivisor(BufferAttribute::INSTANCE_MAT_1, 1);
            m_renderInstanceBuffers[mesh->GetMesh()].AddAtributeDivisor(BufferAttribute::INSTANCE_MAT_2, 1);
            m_renderInstanceBuffers[mesh->GetMesh()].AddAtributeDivisor(BufferAttribute::INSTANCE_MAT_3, 1);
            m_renderInstanceBuffers[mesh->GetMesh()].AddAtributeDivisor(BufferAttribute::INSTANCE_MAT_4, 1);
            submesh->GetVAO()->Unbind();
        }
        m_renderInstanceBuffers[mesh->GetMesh()].Unbind();

        return;
    }

    for(auto& renderable : renderList) {
        if(mesh->GetName() == renderable.second.first->GetName()) {
            m_renderList[mesh->GetMesh()].second += 1;
            m_renderTranforms[mesh->GetMesh()].push_back(mesh->GetComponent<Transform>()->GetTransform());

            auto transformList = m_renderTranforms[mesh->GetMesh()];
            m_renderInstanceBuffers[mesh->GetMesh()].FillBuffer(transformList.size() * sizeof(glm::mat4), &transformList[0], DYNAMIC);
        }
        else {
            m_renderList.emplace(std::make_pair(mesh->GetMesh(), std::make_pair(mesh, 1)));
            m_renderTranforms[mesh->GetMesh()].push_back(mesh->GetComponent<Transform>()->GetTransform());

            auto transformList = m_renderTranforms[mesh->GetMesh()];

            m_renderInstanceBuffers[mesh->GetMesh()].Create(VBO);
            m_renderInstanceBuffers[mesh->GetMesh()].Bind();
            m_renderInstanceBuffers[mesh->GetMesh()].FillBuffer(transformList.size() * sizeof(glm::mat4), &transformList[0], DYNAMIC);

            auto submeshList = mesh->GetSubMeshList();
            for (auto& submesh : submeshList) {
                submesh->GetVAO()->Bind();
                m_renderInstanceBuffers[mesh->GetMesh()].AddAttributePointer(BufferAttribute::INSTANCE_MAT_1, 4, VT_FLOAT, 4 * sizeof(glm::vec4));
                m_renderInstanceBuffers[mesh->GetMesh()].AddAttributePointer(BufferAttribute::INSTANCE_MAT_2, 4, VT_FLOAT, 4 * sizeof(glm::vec4), sizeof(glm::vec4));
                m_renderInstanceBuffers[mesh->GetMesh()].AddAttributePointer(BufferAttribute::INSTANCE_MAT_3, 4, VT_FLOAT, 4 * sizeof(glm::vec4), 2 * sizeof(glm::vec4));
                m_renderInstanceBuffers[mesh->GetMesh()].AddAttributePointer(BufferAttribute::INSTANCE_MAT_4, 4, VT_FLOAT, 4 * sizeof(glm::vec4), 3 * sizeof(glm::vec4));

                m_renderInstanceBuffers[mesh->GetMesh()].AddAtributeDivisor(BufferAttribute::INSTANCE_MAT_1, 1);
                m_renderInstanceBuffers[mesh->GetMesh()].AddAtributeDivisor(BufferAttribute::INSTANCE_MAT_2, 1);
                m_renderInstanceBuffers[mesh->GetMesh()].AddAtributeDivisor(BufferAttribute::INSTANCE_MAT_3, 1);
                m_renderInstanceBuffers[mesh->GetMesh()].AddAtributeDivisor(BufferAttribute::INSTANCE_MAT_4, 1);
                submesh->GetVAO()->Unbind();
            }
            m_renderInstanceBuffers[mesh->GetName()].Unbind();
        }
    }
}

std::vector<GameObject*> Scene::GetGameObjects()
{
    std::vector<GameObject*> returnList;
    for(auto& obj : m_gameObjectList)
    {
        returnList.push_back(obj.get());
    }
    return returnList;
}

void Scene::RemoveGameObject(GameObject* obj)
{
    int index = 0;
    for(int i = 0; i < m_gameObjectList.size(); i++)
    {
        if(obj == m_gameObjectList[i].get())
        {
            GameObject::Destroy(m_gameObjectList[i]);
            index = i;
        }
    }
    m_gameObjectList.erase(std::remove_if(m_gameObjectList.begin(), m_gameObjectList.end(),
        [](std::shared_ptr<GameObject> &ptr) {return ptr == nullptr; }), m_gameObjectList.end());
}
