#include "Engine.h"
#include "LogManager.h"
#include "StateManager.h"
#include "InputManager.h"
#include "ScreenManager.h"

#include "Timing.h"


Engine::Engine()
{
}


Engine::~Engine()
{
}

bool Engine::Initialize(float width, float height, const std::string & title, const std::string& logFile)
{
    Logger::Instance()->Initialize(logFile);
    Screen::Instance()->SetVersion();
    if (!Screen::Instance()->Initialize(title, glm::vec2(width, height))) {
        Logger::Instance()->LogError("Screen: Failed to Initialize correctly!");
        return false;
    }
    Screen::Instance()->InitializeProjection();
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

    StateMachine::Instance()->Render();

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
    Screen::Instance()->Close();
    Logger::Instance()->Shutdown();
    return false;
}
