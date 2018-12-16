#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include "GameObject.h"
#include "Camera.h"
#include "Skybox.h"
#include "Buffer.h"
#include "Mesh.h"

#include <map>
#include <vector>
#include <memory>

#include <CEREAL/archives/xml.hpp>
#include <CEREAL/types/string.hpp>
#include <CEREAL/types/vector.hpp>

class ATOM_API Scene
{
public:
    Scene();
    ~Scene();

    void Initialize();
    void Update(float delta);
    void Render();

    void Reload(std::shared_ptr<Scene>* loadedScene);

    Camera* GetSceneCamera();
    std::shared_ptr<GameObject> GetSceneCameraObject();

    void SetName(const std::string& name);
    std::string GetName();

    void SetSkybox(std::string name);

    void AddGameObject();
    void AddMesh(Mesh* mesh);
    void RemoveMesh(Mesh* mesh);

    std::vector<GameObject*> GetGameObjects();
    std::vector<std::shared_ptr<GameObject>> GetGameObjectPointers();
    void RemoveGameObject(GameObject* obj);

private:
    std::string m_name;

    std::shared_ptr<GameObject> m_sceneCamera;
    std::shared_ptr<Skybox> m_skybox;

    UniformBuffer m_matriceBuffer;
    std::vector<std::shared_ptr<GameObject>> m_gameObjectList;

    std::map<std::string, std::pair<Mesh*, int>> m_renderList;
    std::map<std::string, std::vector<glm::mat4>> m_renderTransforms;
    std::map<std::string, Buffer> m_renderInstanceBuffers;

    friend class cereal::access;

    template<class Archive>
    void serialize(Archive& archive) {
        archive(cereal::make_nvp("Name", m_name), cereal::make_nvp("Skybox", m_skybox), cereal::make_nvp("GameObjectList", m_gameObjectList));
    }
};
