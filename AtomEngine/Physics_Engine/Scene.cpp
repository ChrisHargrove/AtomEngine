#include "Scene.h"
#include "Transform.h"
#include "UniformBufferTypes.h"

#include "ScreenManager.h"
#include "ShaderManager.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "ThreadPool.h"

Scene::Scene(): 
m_sceneCamera(nullptr),
m_name("Scene")
{
}

Scene::~Scene()
{
}

void Scene::Initialize()
{
    //LOAD A SCENE CAMERA IF NONE EXIST
    if(!m_sceneCamera) {
        m_sceneCamera = std::make_shared<GameObject>();
        m_sceneCamera->AddComponent<Transform>();
        m_sceneCamera->AddComponent<Camera>();
        m_sceneCamera->Initialize();
    }

    std::vector<Mesh*> meshList;

    //LOOP THROUGH ALL GAME OBJECTS
    for(auto& obj : m_gameObjectList) {
        //INITIALIZE EACH ONE
        obj->Initialize();
        //IF THE OBJECT HAS A MESH STORE IT
        if(obj->GetComponent<Mesh>() != nullptr) {
            meshList.push_back(obj->GetComponent<Mesh>());
        }
    }

    //LOOP THROUGH ALL MESHES
    std::string meshName = "";
    for(auto& mesh : meshList) {
        //IF A MESH IS BEING USED MORE THAN ONCE INCREMENT ITS COUNT
        //AND STORE ITS TRANSFORM IN A LIST OF TRANSFORMS
        if(meshName == mesh->GetMesh()) {
            m_renderList[meshName].second += 1;
            m_renderTransforms[meshName].push_back(mesh->GetComponent<Transform>()->GetTransformPtr());
        }
        //IF ITS THE FIRST MESH OF THIS OF THIS NAME STORE IT AND IVE IT A COUNT OF 1
        //AND STORE ITS TRANSFORM TOO
        else {
            meshName = mesh->GetMesh();
            m_renderList.emplace(std::make_pair(meshName, std::make_pair(mesh, 1)));
            m_renderTransforms[meshName].push_back(mesh->GetComponent<Transform>()->GetTransformPtr());
        }
    }

    //SET UP A COMMON MATRIX BUFFER FOR VIEW AND PROJECTION
    MatriceBuffer matrixBuffer = { Screen::Instance()->GetProjection(), m_sceneCamera->GetComponent<Camera>()->GetViewMatrix() };
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
        std::vector<glm::mat4*> transformList;
        transformList = m_renderTransforms[mesh.first];
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

void Scene::Update(float delta)
{
    m_sceneCamera->Update(delta);

    if(!m_gameObjectList.empty()) {
        //Get Number of threads running in job system, then calculate how many game objects
        //can be run on each, in parallel
        auto numThreads = JobSystem::Instance()->m_numThreads;
        auto numObj = m_gameObjectList.size() / numThreads;
        if(numObj == 0) {
            numObj = m_gameObjectList.size() % numThreads;
        }

        //Get the game objects as raw pointers instead of smart pointers
        auto objs = GetGameObjects();
        std::vector<GameObject*>::iterator start;
        std::vector<GameObject*>::iterator end;

        //used to store the last job started, so that the update function can wait on it,
        //until all objects are updated.
        std::vector<std::future<void>> jobCompletion;

        bool skipUnnecessary = false;

        //set up a job on each thread
        for (int i = 0; i < numThreads; i++)
        {
            if (skipUnnecessary) continue;

            //calculate the start point of object range for each job
            if (i == 0) { start = objs.begin(); }
            else if (numObj < numThreads) { start = objs.begin(); skipUnnecessary = true; }
            else { start = std::next(objs.begin(), numObj * i); }

            //do the same for the end of the range
            if (i == numThreads) { end = objs.end(); }
            else if(numObj < numThreads) { end = objs.end(); }
            else { end = std::next(objs.begin(), numObj * (i + 1)); }

            //copy the new range into a new vector to pass to the job.
            auto shortObjectList = std::vector<GameObject*>(start, end);

            //Setup a job that will iterate over the short object range and update them.
            auto job = [shortObjectList, delta] () {
                for (auto o : shortObjectList) {
                    o->Update(delta);
                }
            };

            //if creating the last job for the object list, then store the returned
            //job value into the std::future 
            if(skipUnnecessary) {
                jobCompletion.push_back(JobSystem::Instance()->AddJob(job));
            }
            else if (i != numThreads - 1) {
                jobCompletion.push_back(JobSystem::Instance()->AddJob(job));
            }
            else {
                jobCompletion.push_back(JobSystem::Instance()->AddJob(job));
            }

        }

        //wait here until the last job in the thread pool has finished.
        while (!AreJobsReady(jobCompletion)) { };
    }
    
    for(auto& mesh : m_renderTransforms)
    {
        //convert all model matrices from pointers to values.
        std::vector<glm::mat4> converted;
        for(auto& transform : mesh.second) {
            converted.push_back(*transform);
        }
        //fill instance buffers with updated transforms.
        m_renderInstanceBuffers[mesh.first].FillBuffer(converted.size() * sizeof(glm::mat4), &converted[0], DYNAMIC);
    }
}

void Scene::Render()
{
    m_matriceBuffer.SetSubData(&MatriceBuffer::m_view, &m_sceneCamera->GetComponent<Camera>()->GetViewMatrix());

    Shaders::Instance()->UseShader("INSTANCE");
    for(auto& mesh : m_renderList) {
        auto instanceCount = mesh.second.second;
        mesh.second.first->Render(instanceCount);
    }

    if(m_skybox) {
        Shaders::Instance()->UseShader("SKYBOX");
        Shaders::Instance()->GetCurrentShader()->SetMat4("model", glm::mat4(1.0f));
        m_skybox->Render();
    }
    
}

void Scene::Reload(std::shared_ptr<Scene>* loadedScene)
{
    //Before Loading a new Scene make sure to remove all old data
    m_gameObjectList.clear();
    m_name.clear();
    m_sceneCamera.reset();
    m_skybox.reset();

    m_renderList.clear();
    m_renderTransforms.clear();

    for(auto& buffer : m_renderInstanceBuffers) {
        buffer.second.Destroy();
    }
    m_renderInstanceBuffers.clear();

    //Now all data is removed, start pushing new scene data
    m_gameObjectList = loadedScene->get()->GetGameObjectPointers();
    m_name = loadedScene->get()->GetName();
    m_sceneCamera = loadedScene->get()->GetSceneCameraObject();
    m_skybox = Resource::Instance()->GetResource<Skybox>(loadedScene->get()->m_skybox->GetName());

    Initialize();
}

Camera* Scene::GetSceneCamera()
{
    return m_sceneCamera->GetComponent<Camera>();
}

std::shared_ptr<GameObject> Scene::GetSceneCameraObject()
{
    return m_sceneCamera;
}

void Scene::SetName(const std::string& name)
{
    m_name = name;
}

std::string Scene::GetName()
{
    return m_name;
}

void Scene::SetSkybox(std::string name)
{
    m_skybox = Resource::Instance()->GetResource<Skybox>(name);
}

void Scene::AddGameObject()
{
    auto obj = std::make_shared<GameObject>();
    obj->AddComponent<Transform>();
    m_gameObjectList.push_back(obj);
    obj->Initialize();
}

void Scene::AddGameObject(std::shared_ptr<GameObject> obj)
{
    m_gameObjectList.push_back(obj);
    obj->Initialize();
}

void Scene::AddMesh(Mesh* mesh)
{
    if(mesh->GetMesh().empty()) {
        return;
    }

    std::map<std::string, std::pair<Mesh*, int>> renderList(m_renderList);

    if(renderList.empty())
    {
        m_renderList.emplace(std::make_pair(mesh->GetMesh(), std::make_pair(mesh, 1)));
        m_renderTransforms[mesh->GetMesh()].push_back(mesh->GetComponent<Transform>()->GetTransformPtr());

        std::vector<glm::mat4> transformList;
        for (auto& transform : m_renderTransforms[mesh->GetMesh()]) {
            transformList.push_back(*transform);
        }

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
        if(mesh->GetMesh() == renderable.second.first->GetMesh()) {
            m_renderList[mesh->GetMesh()].second += 1;
            m_renderTransforms[mesh->GetMesh()].push_back(mesh->GetComponent<Transform>()->GetTransformPtr());

            std::vector<glm::mat4> transformList;
            for (auto& transform : m_renderTransforms[mesh->GetMesh()]) {
                transformList.push_back(*transform);
            }
            m_renderInstanceBuffers[mesh->GetMesh()].FillBuffer(transformList.size() * sizeof(glm::mat4), &transformList[0], DYNAMIC);
        }
        else {
            m_renderList.emplace(std::make_pair(mesh->GetMesh(), std::make_pair(mesh, 1)));
            m_renderTransforms[mesh->GetMesh()].push_back(mesh->GetComponent<Transform>()->GetTransformPtr());
            std::vector<glm::mat4> transformList;
            for (auto& transform : m_renderTransforms[mesh->GetMesh()]) {
                transformList.push_back(*transform);
            }

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

void Scene::RemoveMesh(Mesh* mesh)
{
    m_renderList.at(mesh->GetMesh()).second--;
    if(m_renderList.at(mesh->GetMesh()).second == 0) {
        m_renderList.erase(mesh->GetMesh());
    }
    m_renderTransforms.at(mesh->GetMesh()).erase(std::remove_if(m_renderTransforms[mesh->GetMesh()].begin(), m_renderTransforms[mesh->GetMesh()].end(),
        [mesh](glm::mat4* transform) { return transform == mesh->GetComponent<Transform>()->GetTransformPtr(); }), m_renderTransforms[mesh->GetMesh()].end());
    if(m_renderTransforms.at(mesh->GetMesh()).empty()) {
        m_renderTransforms.erase(mesh->GetMesh());
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

std::vector<std::shared_ptr<GameObject>> Scene::GetGameObjectPointers()
{
    return m_gameObjectList;
}

void Scene::RemoveGameObject(GameObject* obj)
{
    int index = 0;
    for(int i = 0; i < m_gameObjectList.size(); i++)
    {
        if(obj == m_gameObjectList[i].get())
        {
            auto mesh = obj->GetComponent<Mesh>();
            if(mesh != nullptr)
            {
                m_renderList.at(mesh->GetMesh()).second--;
                if(m_renderList.at(mesh->GetMesh()).second <= 0)
                {
                    m_renderList.erase(mesh->GetMesh());
                }
                m_renderTransforms.at(mesh->GetMesh()).erase(std::remove_if(m_renderTransforms[mesh->GetMesh()].begin(), m_renderTransforms[mesh->GetMesh()].end(),
                    [obj](glm::mat4* transform) { return transform == obj->GetComponent<Transform>()->GetTransformPtr(); }), m_renderTransforms[mesh->GetMesh()].end());
                if(m_renderTransforms.at(mesh->GetMesh()).empty())
                {
                    m_renderTransforms.erase(mesh->GetMesh());
                }
            }

            GameObject::Destroy(m_gameObjectList[i]);
            m_gameObjectList.erase(std::remove(m_gameObjectList.begin(), m_gameObjectList.end(), m_gameObjectList[i]), m_gameObjectList.end());
            index = i;
        }
    }
    m_gameObjectList.erase(std::remove_if(m_gameObjectList.begin(), m_gameObjectList.end(),
        [](std::shared_ptr<GameObject> &ptr) {return ptr == nullptr; }), m_gameObjectList.end());
}
