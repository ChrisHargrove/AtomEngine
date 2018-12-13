#pragma once

#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_sdl.h"
#include "IMGUI/imgui_impl_opengl3.h"
#include <memory>

class Component;
class GameObject;
class Scene;

enum class WindowID
{
    HEIRARCHY = 0,
    OBJECT_EDITOR,
    SAVE_AS,
    LOAD
};

class GUIManager
{
public:
    static GUIManager* Instance() {
        if(m_instance == nullptr) {
            m_instance = new GUIManager();
        }
        return m_instance;
    }

    void Initialize();
    void StartFrame();
    void Render();
    void ProcessInput(SDL_Event* evt);
    void Shutdown();

    void CloseWindow(WindowID id);

    void ShowGUI();
    bool QuitCalled();

    void SetSceneData(std::shared_ptr<Scene>* scene);
    Scene* GetScene();

private:
    GUIManager() {};
    GUIManager(const GUIManager&) {};
    ~GUIManager() {};

    bool m_quitCalled;
    bool m_showHeirarchyWindow;
    bool m_showObjectEditorWindow;
    bool m_showSaveWindow;
    bool m_showLoadWindow;

    std::shared_ptr<Scene>* m_scene;

    //Heirachy Window Variables 
    int m_nodeClicked;
    long int m_selectionMask;
    GameObject* m_objectSelected;
    ImGuiTreeNodeFlags m_nodeFlags;

    //Object Editor Window Variables
    char m_nameBuffer[128] = "";
    char m_meshName[64] = "";

    void ShowGameObjectNode(GameObject* obj, int& index);
    void ShowMenuBar();
    void ShowMenu();
    void ShowHeirarchyWindow();
    void ShowObjectEditorWindow();
    void ShowSaveWindow();
    void ShowLoadWindow();

    void ShowComponontInfo(Component* component);

    static GUIManager* m_instance;
};

typedef GUIManager GUI;