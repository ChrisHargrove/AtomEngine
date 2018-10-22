#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include "Singleton.h"  
#include "AlignedAllocation.h"

#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_sdl.h"
#include "IMGUI/imgui_impl_opengl3.h"

class GUIManager : public AlignedAllocation<BYTE16>
{
public:
    friend class Singleton<GUIManager>;

    void Initialize();

    void StartFrame();
    void Render();

    void ProcessInput(SDL_Event* evt);

    void Shutdown();

private:
    GUIManager() {};
    GUIManager(const GUIManager&) {};
    ~GUIManager() {};
};

template ATOM_API GUIManager* Singleton<GUIManager>::Instance();

typedef Singleton<GUIManager> GUI;