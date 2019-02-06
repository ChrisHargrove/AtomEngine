#include "Engine.h"
#include "LogManager.h"
#include "StateManager.h"
#include "InputManager.h"
#include "ScreenManager.h"

#include "Timing.h"
#include "ProfilerManager.h"

Engine::Engine()
{
}


Engine::~Engine()
{
}

bool Engine::Initialize(float width, float height, const std::string & title, const std::string& logFile)
{
    Profiler::Instance()->Initialize("AtomProfile.log");
    Logger::Instance()->Initialize(logFile);
    Screen::Instance()->SetVersion();
    if (!Screen::Instance()->Initialize(title, glm::vec2(width, height))) {
        Logger::Instance()->LogError("Screen: Failed to Initialize correctly!");
        return false;
    }
    Screen::Instance()->InitializeProjection();

    if (m_guiInitializeCallback != nullptr) m_guiInitializeCallback();

    return false;
}

bool Engine::Initialize(const std::string & configFile)
{

    return false;
}

void Engine::Input()
{
    Input::Instance()->Update();
    StateMachine::Instance()->Input();
}

void Engine::Update(float delta)
{
    StateMachine::Instance()->Update(delta);
}

void Engine::Render()
{
    Screen::Instance()->Clear();

    if (m_guiStartFrameCallback != nullptr) m_guiStartFrameCallback();

    StateMachine::Instance()->Render();

    if(m_guiRenderFrameCallback != nullptr) m_guiRenderFrameCallback();

    Screen::Instance()->Swap();
}

int Engine::Run()
{
    Timer* m_timer = new Timer();
    m_timer->Start();

    while (!Input::Instance()->HasQuit()) {
        Input();
        Update(m_timer->GetDelta());
        Render();
    }

    if (!Shutdown()) {
        return -1;
    }

    delete m_timer;
    return 0;
}

bool Engine::Shutdown()
{
    if (!StateMachine::Instance()->Shutdown()) {
        Logger::Instance()->LogError("Game Engine Shutdown Failed!...");
        return false;
    }

    if (m_guiShutdownCallback != nullptr) m_guiShutdownCallback();

    Screen::Instance()->Close();
    Logger::Instance()->Shutdown();
    return true;
}

void Engine::SetGUIInitializeCallback(std::function<void()> callback)
{
    m_guiInitializeCallback = callback;
}

void Engine::SetGUIShutdownCallback(std::function<void()> callback)
{
    m_guiShutdownCallback = callback;
}

void Engine::SetGUIStartFrameCallback(std::function<void()> callback)
{
    m_guiStartFrameCallback = callback;
}

void Engine::SetGUIRenderFrameCallback(std::function<void()> callback)
{
    m_guiRenderFrameCallback = callback;
}
