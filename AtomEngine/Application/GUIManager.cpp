#include "GUIManager.h"

#include "ScreenManager.h"
#include "LogManager.h"
#include "InputManager.h"

#include "CollisionDetectionSystem.h"
#include "RenderDebugSystem.h"

GUIManager* GUIManager::m_instance = nullptr;

void GUIManager::Initialize()
{
    //Set All Booleans
    m_quitCalled = false;

    m_renderDebugSystem = nullptr;
    m_collisionDetectionSystem = nullptr;

    //SETUP IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplSDL2_InitForOpenGL(Screen::Instance()->GetWindow(), Screen::Instance()->GetContext());
    ImGui_ImplOpenGL3_Init("#version 330");
    //SETUP STYLE
    ImGui::StyleColorsDark();

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
    if(m_quitCalled) Input::Instance()->RequestQuit();
}

void GUIManager::Shutdown()
{
    //CLEANUP IMGUI
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void GUIManager::SetRenderDebugSystem(RenderDebugSystem* renderDebug)
{
    m_renderDebugSystem = renderDebug;
}

void GUIManager::SetCollisionDetectionSystem(CollisionDetectionSystem* bvh)
{
    m_collisionDetectionSystem = bvh;
}

void GUIManager::ShowGUI()
{
    ShowMenuBar();
}

bool GUIManager::QuitCalled()
{
    return m_quitCalled;
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

        if(ImGui::BeginMenu("Physics"))
        {
            if(m_renderDebugSystem) {
                ImGui::Checkbox("Draw AABB's", m_renderDebugSystem->GetShowDebug());
                ImGui::Separator();
            }
            if (m_collisionDetectionSystem) {
                ImGui::Checkbox("Draw BVH Tree", m_collisionDetectionSystem->GetShowBroadphaseDebug());
                ImGui::Separator();
            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void GUIManager::ShowMenu()
{
    if(ImGui::MenuItem("Quit"))
    {
        m_quitCalled = true;
    }
}
