#include "Scene.h"
#include "Transform.h"
#include "UniformBufferTypes.h"

#include "ScreenManager.h"
#include "ShaderManager.h"

Scene::Scene(): 
m_mainCamera(nullptr)
{
}


Scene::~Scene()
{
}

void Scene::Initialize()
{
    //LOAD SKYBOX
    /*m_skybox = new Skybox();
    m_skybox->Load("OM_skybox/OM.jpg");*/

    if(m_skybox == nullptr)
    {
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

    std::vector<Mesh*> meshList;

    //LOOP THROUGH ALL GAME OBJECTS
    for(auto& obj : m_gameObjectList)
    {
        //INITIALIZE EACH ONE
        obj->Initialize();
        //IF THE OBJECT HAS A CAMERA ATTACHED STORE A POINTER TO IT
        if(obj->GetComponent<Camera>() != nullptr)
        {
            m_mainCamera = obj->GetComponent<Camera>();
        }
        //IF THE OBJECT HAS A MESH STORE IT
        if(obj->GetComponent<Mesh>() != nullptr)
        {
            meshList.push_back(obj->GetComponent<Mesh>());
        }
    }
    //IF NO GAME CAMERA FOUND ASSIGN SCENE CAM TO IT
    if(m_mainCamera == nullptr)
    {
        m_gameObjectList.push_back(sceneCam);
        m_mainCamera = m_sceneCamera.get();
    }

    //LOOP THROUGH ALL MESHES
    std::string meshName = "";
    for(auto& mesh : meshList)
    {
        //IF A MESH IS BEING USED MORE THAN ONCE INCREMENT ITS COUNT
        //AND STORE ITS TRANSFORM IN A LIST OF TRANSFORMS
        if(meshName == mesh->GetName())
        {
            m_renderList[meshName].second += 1;
            m_renderTranforms[meshName].push_back(mesh->GetComponent<Transform>()->GetTransform());

        }
        //IF ITS THE FIRST MESH OF THIS OF THIS NAME STORE IT AND IVE IT A COUNT OF 1
        //AND STORE ITS TRANSFORM TOO
        else
        {
            meshName = mesh->GetName();
            m_renderList.emplace(std::make_pair(meshName, std::make_pair(mesh, 1)));
            m_renderTranforms[meshName].push_back(mesh->GetComponent<Transform>()->GetTransform());
        }
    }

    //SET UP A COMMON MATRIX BUFFER FOR VIEW AND PROJECTON
    MatriceBuffer matrixBuffer = { Screen::Instance()->GetProjection(), m_sceneCamera->GetViewMatrix() };
    Shaders::Instance()->SetUniformBufferBinding("Matrices", UniformBufferBinding::MATRICES);

    m_matriceBuffer.Create(sizeof(matrixBuffer));
    m_matriceBuffer.BindBuffer(UniformBufferBinding::MATRICES);
    m_matriceBuffer.SetSubData(&MatriceBuffer::m_projection, &matrixBuffer.m_projection);
    m_matriceBuffer.SetSubData(&MatriceBuffer::m_view, &matrixBuffer.m_view);

    //LOOP THROUGH ALL RENDERABLE MESHES
    for(auto& mesh : m_renderList)
    {
        //CREATE A VERTEX BUFFER FOR THE MESH TRANSFORMS
        m_renderInstanceBuffers[mesh.first].Create(VBO);
        m_renderInstanceBuffers[mesh.first].Bind();
        std::vector<glm::mat4> transformList;
        m_renderInstanceBuffers[mesh.first].FillBuffer(transformList.size() * sizeof(glm::mat4), &transformList[0], STATIC);
        //FILL THE BUFFER WITH THE MESHES TRANSFORMS
        auto submeshList = mesh.second.first->GetSubMeshList();
        //LOOP THROUGH EACH SUBMESH AND BIND ITS VAO THEN CONFIGURE THE TRANSFORM BUFFER FOR INSTANCE
        //RENDERING
        for(auto& submesh : submeshList)
        {
            submesh->GetVAO()->Bind();
            m_instanceBuffer.AddAttributePointer(BufferAttribute::INSTANCE_MAT_1, 4, VT_FLOAT, sizeof(glm::vec4));
            m_instanceBuffer.AddAttributePointer(BufferAttribute::INSTANCE_MAT_2, 4, VT_FLOAT, sizeof(glm::vec4), sizeof(glm::vec4));
            m_instanceBuffer.AddAttributePointer(BufferAttribute::INSTANCE_MAT_3, 4, VT_FLOAT, sizeof(glm::vec4), 2 * sizeof(glm::vec4));
            m_instanceBuffer.AddAttributePointer(BufferAttribute::INSTANCE_MAT_4, 4, VT_FLOAT, sizeof(glm::vec4), 3 * sizeof(glm::vec4));

            m_instanceBuffer.AddAtributeDivisor(BufferAttribute::INSTANCE_MAT_1, 1);
            m_instanceBuffer.AddAtributeDivisor(BufferAttribute::INSTANCE_MAT_2, 1);
            m_instanceBuffer.AddAtributeDivisor(BufferAttribute::INSTANCE_MAT_3, 1);
            m_instanceBuffer.AddAtributeDivisor(BufferAttribute::INSTANCE_MAT_4, 1);
            submesh->GetVAO()->Unbind();
        }
        m_renderInstanceBuffers[mesh.first].Unbind();
    }

}

void Scene::Update()
{
    
}


void Scene::Render()
{
    m_matriceBuffer.SetSubData(&MatriceBuffer::m_view, &m_sceneCamera->GetViewMatrix());

    Shaders::Instance()->UseShader("INSTANCE");
    for(auto& mesh : m_renderList)
    {
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

std::vector<GameObject*> Scene::GetGameObjects()
{
    std::vector<GameObject*> returnList;
    for(auto& obj : m_gameObjectList)
    {
        returnList.push_back(obj.get());
    }
    return returnList;
}
