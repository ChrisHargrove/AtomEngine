#include "InputManager.h"
#include "ScreenManager.h"
#include "LogManager.h"
#include <SDL/SDL.h>


void InputManager::Update()
{
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        //GUI::Instance()->ProcessInput(&e);
        if(m_guiInputCallback != nullptr) m_guiInputCallback(&e);

        switch (e.type) {
        case SDL_KEYDOWN:
            AddKeyboardEvent(e.key.keysym.sym, e.key.state, e.key.keysym.mod);
            break;
        case SDL_KEYUP:
            AddKeyboardEvent(e.key.keysym.sym, e.key.state, e.key.keysym.mod);
            break;
        case SDL_WINDOWEVENT:
            AddWindowEvent(e.window.windowID, e.window.event, e.window.data1, e.window.data2);
            break;
        case SDL_MOUSEBUTTONDOWN:
            AddMouseButtonEvent(e.button.button, e.button.state, e.button.clicks);
            break;
        case SDL_MOUSEBUTTONUP:
            AddMouseButtonEvent(e.button.button, e.button.state, e.button.clicks);
            break;
        case SDL_MOUSEMOTION:
            AddMouseMoveEvent(e.motion.x, e.motion.y, e.motion.xrel, e.motion.yrel);
            break;
        case SDL_MOUSEWHEEL:
            AddMouseScrollEvent(e.wheel.x, e.wheel.y, e.wheel.direction);
            break;
        case SDL_QUIT:
            AddWindowEvent(e.window.windowID, e.type, 0, 0);
            break;
        }
    }
}

bool InputManager::IsKeyPressed(int key)
{
    auto found = m_keyboardEvents.find(key);
    if (found != m_keyboardEvents.end()) {
        if (m_keyboardEvents.at(key).m_state == SDL_PRESSED && m_keysPressed.at(key) != true) {
            m_keysPressed.at(key) = true;
            return true;
        }
    }
    return false;
}

bool InputManager::IsKeyReleased(int key)
{
    auto found = m_keyboardEvents.find(key);
    if (found != m_keyboardEvents.end()) {
        if (m_keyboardEvents.at(key).m_state == SDL_RELEASED) {
            return true;
        }
    }
    return false;
}

bool InputManager::IsKeyHeld(int key)
{
    auto found = m_keyboardEvents.find(key);
    if (found != m_keyboardEvents.end()) {
        if (m_keyboardEvents.at(key).m_state == SDL_PRESSED && m_keysPressed.at(key) == true) {
            return true;
        }
        else if (m_keyboardEvents.at(key).m_state == SDL_PRESSED) {
            return true;
        }
    }
    return false;
}

bool InputManager::IsButtonClicked(int button)
{
    auto found = m_mouseButtonEvents.find(button);
    if (found != m_mouseButtonEvents.end()) {
        if (m_mouseButtonEvents.at(button).m_state == SDL_PRESSED && m_mouseButtonEvents.at(button).m_held == false) {
            m_mouseButtonEvents.at(button).m_held = true;
            return true;
        }
    }
    return false;
}

bool InputManager::IsButtonDoubleClicked(int button)
{
    auto found = m_mouseButtonEvents.find(button);
    if (found != m_mouseButtonEvents.end()) {
        if (m_mouseButtonEvents.at(button).m_state == SDL_PRESSED && m_mouseButtonEvents.at(button).m_clicks > 1 && m_mouseButtonEvents.at(button).m_doubleClicked == false) {
            m_mouseButtonEvents.at(button).m_doubleClicked = true;
            return true;
        }
    }
    return false;
}

bool InputManager::IsButtonReleased(int button)
{
    auto found = m_mouseButtonEvents.find(button);
    if (found != m_mouseButtonEvents.end()) {
        if (m_mouseButtonEvents.at(button).m_state == SDL_RELEASED) {
            m_mouseButtonEvents.erase(button);
            return true;
        }
    }
    return false;
}

bool InputManager::IsButtonHeld(int button)
{
    auto found = m_mouseButtonEvents.find(button);
    if (found != m_mouseButtonEvents.end()) {
        if (m_mouseButtonEvents.at(button).m_state == SDL_PRESSED) {
            return true;
        }
    }
    return false;
}

bool InputManager::HasMouseMoved()
{
    bool r = m_mouseMoveEvent.m_hasMoved;
    m_mouseMoveEvent.m_hasMoved = false;
    return r;
}

const MouseMoveEvent & InputManager::GetMouseMove()
{
    return m_mouseMoveEvent;
}

bool InputManager::HasScrolled()
{
    bool r = m_mouseScrollEvent.m_hasMoved;
    m_mouseScrollEvent.m_hasMoved = false;
    return r;
}

const MouseScrollEvent & InputManager::GetMouseScroll()
{
    return m_mouseScrollEvent;
}

bool InputManager::HasQuit()
{
    if (CheckForWinEvent(SDL_QUIT)) {
        return true;
    }
    return false;
}

void InputManager::RequestQuit()
{
    AddWindowEvent(0, SDL_QUIT, 0, 0);
}

bool InputManager::CheckForWinEvent(int windowEvent)
{
    auto found = m_windowEvents.find(windowEvent);
    if (found != m_windowEvents.end()) {
        return true;
    }
    return false;
}

WindowEvent InputManager::GetWinEvent(int windowEvent)
{
    auto found = m_windowEvents.find(windowEvent);
    if (found != m_windowEvents.end()) {
        return m_windowEvents.at(windowEvent);
    }
    return WindowEvent(0, SDL_WINDOWEVENT_FOCUS_GAINED, 0, 0);
}

void InputManager::CaptureMouse(bool capture)
{
    if (capture) {
        m_isMouseCaptured = true;
    }
    else {
        m_isMouseCaptured = false;
    }
    Screen::Instance()->CaptureMouse(capture);
}

bool InputManager::IsMouseCaptured()
{
    return m_isMouseCaptured;
}

void InputManager::SetGUICallback(std::function<void(SDL_Event* evt)> callback)
{
    m_guiInputCallback = callback;
}

void InputManager::AddKeyboardEvent(int keyCode, int state, int modifier)
{
    KeyEvent kEvent(keyCode, state, modifier);
    auto found = m_keyboardEvents.find(keyCode);
    if (found != m_keyboardEvents.end()) {
        m_keyboardEvents.at(keyCode) = kEvent;
        if (state == SDL_RELEASED) {
            m_keysPressed.at(keyCode) = false;
        }
    }
    else {
        m_keyboardEvents.emplace(std::make_pair(keyCode, kEvent));
        m_keysPressed.emplace(std::make_pair(keyCode, false));
    }
}

void InputManager::AddWindowEvent(int windowID, int type, int data1, int data2)
{
    WindowEvent wEvent(windowID, type, data1, data2);
    auto found = m_windowEvents.find(type);
    if (found != m_windowEvents.end()) {
        m_windowEvents.at(type) = wEvent;
    }
    else {
        m_windowEvents.emplace(std::make_pair(type, wEvent));
    }
}

void InputManager::AddMouseScrollEvent(double scrollX, double scrollY, int direction)
{
    m_mouseScrollEvent = MouseScrollEvent(direction, scrollX, scrollY);
    if (direction == SDL_MOUSEWHEEL_FLIPPED) {
        m_mouseScrollEvent.m_scrollAmountX *= -1;
        m_mouseScrollEvent.m_scrollAmountY *= -1;
    }
    m_mouseScrollEvent.m_hasMoved = true;
}

void InputManager::AddMouseMoveEvent(double positionX, double positionY, double relativeX, double relativeY)
{
    m_mouseMoveEvent = MouseMoveEvent(positionX, positionY, relativeX, relativeY);
    m_mouseMoveEvent.m_hasMoved = true;
}

void InputManager::AddMouseButtonEvent(int button, int state, int clicks)
{
    MouseButtonEvent mEvent(button, state, clicks);
    auto found = m_mouseButtonEvents.find(button);
    if (found != m_mouseButtonEvents.end()) {
        m_mouseButtonEvents.at(button) = mEvent;
    }
    else {
        m_mouseButtonEvents.emplace(std::make_pair(button, mEvent));
    }
}
