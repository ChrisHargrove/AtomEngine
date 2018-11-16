/*!
    * \class Engine "Engine.h"
    * \brief Creates the main game engine.
    *
    * Creates the game engine and handles the main gaim loop, which in turn will interface with
    * the StateManager to enable smooth running of the state machine, to call the current game state
    * through polymorphism.
*/
#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include "AlignedAllocation.h"

#include <string>
#include <functional>

class ATOM_API Engine : public AlignedAllocation<BYTE16>
{
public:
    Engine();
    ~Engine();

    /*!
        * \brief Initializes game engine using parameters supplied.
        * \param width How wide the window needs to be in pixels.
        * \param height How tall the window needs to be in pixels.
        * \param title The title of the window.
        * \param logFile The name of the file to log to.
    */
    bool Initialize(float width, float height,const std::string& title, const std::string& logFile);

    /*!
        * \brief Initializes game engine using a config file.
        * \param configFile The file to read the starting values required for the engine.
        *
        * Initializes the game engine using a configuration file that contains information
        * such as Width, Height, Title, amongst other values to get the game engine running.
    */
    bool Initialize(const std::string& configFile);

    /*!
        * \brief Calls the StateManager Input() function.
    */
    void Input();

    /*!
        * \brief Calls the StateManager Update(float delta) function.
        * \param delta The time passed since last frame.
    */
    void Update(float delta);

    /*!
        * \brief Calls the StateManager Render() function.
    */
    void Render();

    /*!
        * \brief Starts the main application loop.
        * \return Returns 0 on successfull end of loop, -1 on error.
    */
    int Run();

    /*!
        * \brief Shuts down the game engine.
        *
        * Calls the shutdown function of the StateMachine and closes the application gently.
    */
    bool Shutdown();

    void SetGUIInitializeCallback(std::function<void()> callback);
    void SetGUIStartFrameCallback(std::function<void()> callback);
    void SetGUIRenderFrameCallback(std::function<void()> callback);
    void SetGUIShutdownCallback(std::function<void()> callback);

private:

    std::function<void()> m_guiInitializeCallback;
    std::function<void()> m_guiShutdownCallback;
    std::function<void()> m_guiStartFrameCallback;
    std::function<void()> m_guiRenderFrameCallback;

};

