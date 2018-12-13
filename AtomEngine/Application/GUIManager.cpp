#include "GUIManager.h"

#include "ScreenManager.h"
#include "LogManager.h"
#include "Scene.h"

#include "GameObject.h"

#include "Transform.h"
#include "Mesh.h"
#include "Camera.h"

#include "IOManager.h"
#include "SerialRegister.h"
#include "SerialExtensions.h"

GUIManager* GUIManager::m_instance = nullptr;

//Renaming Field
struct callback {
    static int RenameObject(ImGuiInputTextCallbackData* data) {
        static_cast<GameObject*>(data->UserData)->SetName(std::string(data->Buf));
        return 0;
    }
    static int RenameMeshPath(ImGuiInputTextCallbackData* data) {
        static_cast<Mesh*>(data->UserData)->SetMesh(std::string(data->Buf));
        if(static_cast<Mesh*>(data->UserData)->Initialize())
        {
            GUI::Instance()->GetScene()->AddMesh(static_cast<Mesh*>(data->UserData));
        }
        return 0;
    }
    static int SaveScene(ImGuiInputTextCallbackData* data) {
        IO::Instance()->Open(std::string(data->Buf), std::ios::out);
        IO::Instance()->Serialize<cereal::XMLOutputArchive>(*static_cast<std::weak_ptr<Scene>*>(data->UserData));
        IO::Instance()->Close();
        GUI::Instance()->CloseWindow(WindowID::SAVE_AS);
        return 0;
    }
};

void GUIManager::Initialize()
{
    //Set All Booleans
    m_quitCalled = false;
    m_showHeirarchyWindow = false;
    m_showObjectEditorWindow = false;
    m_showSaveWindow = false;

    //SETUP IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplSDL2_InitForOpenGL(Screen::Instance()->GetWindow(), Screen::Instance()->GetContext());
    ImGui_ImplOpenGL3_Init("#version 330");
    //SETUP STYLE
    ImGui::StyleColorsDark();

    //Heirarchy Viewer Setup
    m_nodeFlags = 0;
    m_selectionMask = 0;
    m_nodeClicked = -1;
    m_objectSelected = nullptr;

    //Object Editor Setup

}

void GUIManager::StartFrame()
{
    //START IMGUI FRAME
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(Screen::Instance()->GetWindow());
    ImGui::NewFrame();
}

void GUIManager::Render()
{
    //RENDER IMGUI FRAME
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUIManager::ProcessInput(SDL_Event* evt)
{
    ImGui_ImplSDL2_ProcessEvent(evt);
}

void GUIManager::Shutdown()
{
    //CLEANUP IMGUI
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void GUIManager::CloseWindow(WindowID id)
{
    switch(id) { 
    case WindowID::HEIRARCHY:
        m_showHeirarchyWindow = false;
        break;
    case WindowID::OBJECT_EDITOR:
        m_showObjectEditorWindow = false;
        break;
    case WindowID::SAVE_AS:
        m_showSaveWindow = false;
        break;
    default: ;
    }
}

void GUIManager::ShowGUI()
{
    ShowMenuBar();

    if (m_showHeirarchyWindow) ShowHeirarchyWindow();
    if (m_showObjectEditorWindow) ShowObjectEditorWindow();
    if (m_showSaveWindow) ShowSaveWindow();
}

bool GUIManager::QuitCalled()
{
    return m_quitCalled;
}

void GUIManager::SetSceneData(std::weak_ptr<Scene> scene)
{
    m_scene = scene;
}

Scene* GUIManager::GetScene()
{
    return m_scene.lock().get();
}

void GUIManager::ShowHeirarchyWindow()
{
    //Handles Early Out if window collapsed.
    if(!ImGui::Begin("Heirarchy"))
    {
        ImGui::End();
        return;
    }

    if(m_scene.lock() != nullptr)
    {
        if(ImGui::TreeNode(m_scene.lock()->GetName().c_str()))
        {
            int index = 0;

            for (auto& obj : m_scene.lock()->GetGameObjects())
            {
                ShowGameObjectNode(obj, index);
                index++;
            }
            ImGui::TreePop();

            if(m_nodeClicked != -1)
            {
                m_selectionMask = (1 << m_nodeClicked);
            }
        }
    }

    //If Right Clicking In Window
    if(ImGui::BeginPopupContextWindow(NULL, 1, false))
    {
        if(ImGui::MenuItem("Add Empty Object"))
        {
            m_scene.lock()->AddGameObject();
        }
        ImGui::EndPopup();
    }

    ImGui::End();
}

void GUIManager::ShowObjectEditorWindow()
{
    if (!ImGui::Begin("Object Editor"))
    {
        ImGui::End();
        return;
    }

    if(m_objectSelected != nullptr)
    {
        ImGui::InputText("Name", m_nameBuffer, 64, ImGuiInputTextFlags_CallbackOnEnter, callback::RenameObject, (void*)m_objectSelected);

        auto componentList = m_objectSelected->GetComponents();

        for(auto component : componentList)
        {
            if(ImGui::CollapsingHeader(component->GetName().c_str()))
            {
                ShowComponontInfo(component);
                
            }else if(ImGui::BeginPopupContextItem())
            {
                if (ImGui::MenuItem("Delete Component"))
                {
                    m_objectSelected->RemoveComponent(component);
                }

                ImGui::EndPopup();
            }

        }

        //If Right Clicking In Window
        if (ImGui::BeginPopupContextWindow(NULL, 1, false))
        {
            if (ImGui::MenuItem("Add Camera"))
            {
                m_objectSelected->AddComponent<Camera>();
            }
            if (ImGui::MenuItem("Add Mesh"))
            {
                m_objectSelected->AddComponent<Mesh>();
            }
            ImGui::EndPopup();
        }
    }

    ImGui::End();
}


void GUIManager::ShowComponontInfo(Component* component)
{
    if(dynamic_cast<Transform*>(component) != nullptr)
    {
        auto ptr = dynamic_cast<Transform*>(component);
        ImGui::DragFloat3("Position", &ptr->GetPosition().x, 0.1f);
        if(ImGui::DragFloat3("Rotation", &ptr->GetEulerAngles().x, 0.1f))
        {
            ptr->SetRotation(glm::quat(glm::radians(ptr->GetEulerAngles())));
        }
        ImGui::DragFloat3("Scale", &ptr->GetScale().x, 0.1f);
    }
    else if (dynamic_cast<Mesh*>(component) != nullptr)
    {
        auto ptr = dynamic_cast<Mesh*>(component);
        ImGui::InputText("Mesh Name", m_meshName, 64, ImGuiInputTextFlags_CallbackOnEnter, callback::RenameMeshPath, (void*)ptr);
    }
    else if (dynamic_cast<Camera*>(component) != nullptr)
    {
        auto ptr = dynamic_cast<Camera*>(component);
    }
}

void GUIManager::ShowGameObjectNode(GameObject* obj, int& index)
{
    m_nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ((m_selectionMask & (1 << index)) ? ImGuiTreeNodeFlags_Selected : 0);

    auto name = obj->GetName() + "##" + std::to_string(index);
    bool open = (ImGui::TreeNodeEx(name.c_str(), m_nodeFlags));

    if (ImGui::IsItemClicked())
    {
        m_objectSelected = obj;
        m_nodeClicked = index;
    }
    else if (ImGui::BeginPopupContextItem())
    {
        if (ImGui::MenuItem("Delete"))
        {
            if(m_objectSelected == obj)
            {
                m_objectSelected = nullptr;
            }
            m_scene.lock()->RemoveGameObject(obj);
        }
        if(ImGui::MenuItem("Add Empty Object"))
        {
            obj->AddChild();
        }
        ImGui::EndPopup();
    }

    if(open)
    {
        for(auto& c_obj : obj->GetChildren())
        {
            index++;
            ShowGameObjectNode(c_obj, index);
        }
        ImGui::TreePop();
    }
    
}

void GUIManager::ShowMenuBar()
{
    if(ImGui::BeginMainMenuBar())
    {
        if(ImGui::BeginMenu("File"))
        {
            ShowMenu();
            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu("Window"))
        {
            ImGui::MenuItem("Heirarchy", NULL, &m_showHeirarchyWindow);
            ImGui::MenuItem("Object Editor", NULL, &m_showObjectEditorWindow);

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void GUIManager::ShowMenu()
{
    if(ImGui::MenuItem("New"))
    {
        
    }
    if(ImGui::MenuItem("Load"))
    {
        
    }
    if(ImGui::MenuItem("Save"))
    {
        m_showSaveWindow = true;
    }
    if(ImGui::MenuItem("Quit"))
    {
        m_quitCalled = true;
    }
}

void GUIManager::ShowSaveWindow()
{
    if (!ImGui::Begin("Save As..."))
    {
        ImGui::End();
        return;
    }

    char path[64] = "";
    if (ImGui::InputText("Path", path, 64, ImGuiInputTextFlags_CallbackOnEnter, callback::SaveScene, &m_scene))
    {

    }

    ImGui::End();
}