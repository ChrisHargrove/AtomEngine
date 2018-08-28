#include "StateManager.h"

void StateManager::Input()
{
    if (m_activeStack.empty()) {
        Logger::Instance()->LogError("No Active State on Input() Call! StateManager.cpp - Line 6.");
        return;
    }
    if (!m_activeStack.top()->IsPaused()) {
        m_activeStack.top()->Input();;
    }
}

void StateManager::Update(float delta)
{
    if (m_activeStack.empty()) {
        Logger::Instance()->LogError("No Active State on Update(delta) Call! StateManager.cpp - Line 17.");
        return;
    }
    if (!m_activeStack.top()->IsPaused()) {
        m_activeStack.top()->Update(delta);
    }
}

void StateManager::Render()
{
    if (m_activeStack.empty()) {
        Logger::Instance()->LogError("No Active State on Render() Call! StateManager.cpp - Line 28.");
        return;
    }
    if (!m_activeStack.top()->IsPaused()) {
        m_activeStack.top()->Render();
    }
}

bool StateManager::Shutdown()
{
    if (m_activeStack.empty()) {
        Logger::Instance()->LogError("No Active State on Shutdown() Call! StateManager.cpp - Line 39.");
        return false;
    }
    while (!m_activeStack.empty()) {
        if (!m_activeStack.top()->Shutdown()) {
            Logger::Instance()->LogError("Top Active State Failed Shutdown! StateManager.cpp - Line 44.");
            return false;
        }
        m_activeStack.pop();
    }
    for (auto state : m_stateList) {
        delete state.second;
    }
    m_stateList.clear();

    Logger::Instance()->LogDebug("State Machine Shutdown Successfull! StateManager.cpp - Line 54.");
    return true;
}

bool StateManager::ChangeState(const std::string & stateName)
{
    auto search = m_stateList.find(stateName);
    if (search == m_stateList.end()) {
        Logger::Instance()->LogError(stateName + "Does Not Exist! StateManager.cpp - Line 62.");
        return false;
    }
    if (m_activeStack.size() == 1) { //Means that only the load state is in the active stack.
        if (!PushState(stateName, false)) {
            Logger::Instance()->LogError(stateName + "Push Failed! StateManager.cpp - Line 67.");
            return false;
        }
    }
    else {
        while (m_activeStack.size() > 1) { //While more than just the load state is on the stack.
            if (!m_activeStack.top()->Shutdown()) {
                Logger::Instance()->LogError("Shutdown Failed on top Active State! StateManager.cpp - Line 74.");
                return false;
            }
            m_activeStack.pop();
            if (!PushState(stateName, false)) {
                Logger::Instance()->LogError(stateName + "Push Failed, during ChangeState() Call! StateManager.cpp - Line 79.");
                return false;
            }
        }
    }
    Logger::Instance()->LogDebug(stateName + "Change successfull!");
    return true;
}

bool StateManager::PushState(const std::string & stateName, bool search)
{
    if (search) {
        auto stateSearch = m_stateList.find(stateName);
        if (stateSearch == m_stateList.end()) {
            Logger::Instance()->LogError(stateName + "Does Not Exist! StateManager.cpp - Line 93.");
            return false;
        }
        m_activeStack.push(m_stateList.at(stateName));
        m_activeStack.push(m_stateList.at(stateName));
        if (m_activeStack.top() != m_stateList.at(stateName)) {
            Logger::Instance()->LogError(stateName + "Push Failed! StateManager.cpp - Line 99.");
            return false;
        }
        if (!m_activeStack.top()->Initialize()) {
            Logger::Instance()->LogError(stateName + "Initialize Failed! StateManager.cpp - Line 103.");
            return false;
        }
    }
    else {
        if (!m_activeStack.empty()) {
            if (!m_activeStack.top()->IsPaused()) {
                m_activeStack.top()->Pause();
            }
        }
        m_activeStack.push(m_stateList.at(stateName));
        if (m_activeStack.top() != m_stateList.at(stateName)) {
            Logger::Instance()->LogError(stateName + "Push Failed! StateManager.cpp - Line 115.");
            return false;
        }
        if (!m_activeStack.top()->Initialize()) {
            Logger::Instance()->LogError(stateName + "Initialize Failed! StateManager.cpp - Line 119.");
            return false;
        }
    }
    Logger::Instance()->LogDebug(stateName + "Push Successfull!");
    return true;
}

bool StateManager::PopState()
{
    if (m_activeStack.empty()) {
        Logger::Instance()->LogError("Active State Stack Empty! StateManager.cpp - Line 130.");
        return false;
    }
    if (!m_activeStack.top()->Shutdown()) {
        Logger::Instance()->LogError("Shutdown during Pop Failed! StateManager.cpp - Line 134.");
        return false;
    }
    m_activeStack.pop();
    if (!m_activeStack.empty()) {
        if (!m_activeStack.top()->IsPaused()) {
            Logger::Instance()->LogWarning("Previous State Wasn't Paused! StateManager.cpp - Line 140.");
        }
        else {
            m_activeStack.top()->Resume();
        }
    }
    return true;
}

bool StateManager::AddState(const std::string & stateName, State * state)
{
    auto search = m_stateList.find(stateName);
    if (search != m_stateList.end()) {
        Logger::Instance()->LogError(stateName + "Already Exists! StateManager.cpp - Line 153.");
        return false;
    }
    m_stateList.emplace(std::make_pair(stateName, state));
    return true;
}

State * StateManager::GetState(const std::string & stateName)
{
    auto search = m_stateList.find(stateName);
    if (search != m_stateList.end()) {
        return m_stateList.at(stateName);
    }
    return nullptr;
}

