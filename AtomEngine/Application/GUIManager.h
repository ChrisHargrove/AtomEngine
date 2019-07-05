#pragma once

#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_sdl.h"
#include "IMGUI/imgui_impl_opengl3.h"
#include <memory>

class CollisionDetectionSystem;
class RenderDebugSystem;

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

    void SetRenderDebugSystem(RenderDebugSystem* renderDebug);
    void SetCollisionDetectionSystem(CollisionDetectionSystem* bvh);

    void ShowGUI();
    bool QuitCalled();

private:
    GUIManager() {};
    GUIManager(const GUIManager&) {};
    ~GUIManager() {};

    bool m_quitCalled;

    void ShowMenuBar();
    void ShowMenu();

    static GUIManager* m_instance;

    RenderDebugSystem* m_renderDebugSystem;
    CollisionDetectionSystem* m_collisionDetectionSystem;
};

typedef GUIManager GUI;