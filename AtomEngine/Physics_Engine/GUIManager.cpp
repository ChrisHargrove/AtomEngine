#include "GUIManager.h"

#include "ScreenManager.h"

void GUIManager::Initialize()
{
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
}

void GUIManager::Shutdown()
{
    //CLEANUP IMGUI
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}
