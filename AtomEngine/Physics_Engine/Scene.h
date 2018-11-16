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
    void Update();
    void Render();

    Camera* GetMainCamera();
    void SetMainCamera(Camera* camera);

    void SetName(const std::string& name);
    std::string GetName();

    void AddGameObject();
    void AddMesh(Mesh* mesh);

    std::vector<GameObject*> GetGameObjects();
    void RemoveGameObject(GameObject* obj);

private:
    std::string m_name;

    Camera* m_mainCamera;

    std::shared_ptr<Camera> m_sceneCamera;
    std::shared_ptr<Skybox> m_skybox;

    Buffer m_instanceBuffer;
    UniformBuffer m_matriceBuffer;

    std::vector<std::shared_ptr<GameObject>> m_gameObjectList;

    std::map<std::string, std::pair<Mesh*, int>> m_renderList;
    std::map<std::string, std::vector<glm::mat4>> m_renderTranforms;
    std::map<std::string, Buffer> m_renderInstanceBuffers;

    friend class cereal::access;

    template<class Archive>
    void serialize(Archive& archive) {
        archive(cereal::make_nvp("Name", m_name), cereal::make_nvp("Skybox", m_skybox), cereal::make_nvp("GameObjectList", m_gameObjectList));
    }
};
