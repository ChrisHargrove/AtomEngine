/*!
    * \class InputManager "InputManager.h"
    * \brief Manager for handling all user input.
    *
    * Handles the storage of all user input events using the SDL API. It handles mouse and keyboard
    * as main input devices, it can also handle joystick/game controller input.
*/
#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include <map>

#include "Singleton.h"
#include "AlignedAllocation.h"

/*!
    * \struct KeyEvent "InputManager.h"
    * \brief Structure for holding key event data.
    * 
    * Holds all necessary data for a key event each time the user presses a key.
*/
struct ATOM_API KeyEvent {
    int m_keyCode; /*!< The integer value corresponding to each key. */
    int m_state; /*!< The state of the key, either up or down. */
    int m_modifier; /*!< Whether a modifier key is presses such a shift. */

    /*!
        * \brief Creates a Key Event struct.
        * \param keyCode The Key being pressed.
        * \param state The Current state of the key, pressed or not.
        * \param modifier The value of any modifier keys being pressed.
    */
    KeyEvent(int keyCode, int state, int modifier) {
        m_keyCode = keyCode;
        m_state = state;
        m_modifier = modifier;
    }
};
/*!
    * \struct WindowEvent "InputManager.h"
    * \brief Struture for holding all window event data.
    *
    * Holds all necessary data for a window event each time the user interacts with the window.
*/
struct ATOM_API WindowEvent {
    int m_windowID; /*!< The ID of the window associated with the event. */
    int m_type; /*!< The type of window event e.g. SDL_QUIT */
    int m_data1; /*!< First data of the event. */
    int m_data2; /*!< Second data of the event. */

    /*!
        * \brief Creates a window event structure.
        * \param windowID The ID of the woindow associated with the event.
        * \param type The type of window event.
        * \param data1 The first piece of window event data.
        * \param data2 The second piece of window event data.
    */
    WindowEvent(int windowID, int type, int data1, int data2) {
        m_windowID = windowID;
        m_type = type;
        m_data1 = data1;
        m_data2 = data2;
    }
};

/*!
    * \struct MouseButtonEvent "InputManager.h"
    * \brief Structure for holding all mouse button event data.
    *
    * Holds all necessary data for a mouse button event, each time the user clicks a mouse button.
*/
struct ATOM_API MouseButtonEvent {
    int m_button; /*!< The button ID associated with the event. */
    int m_state; /*!< The state of the button, Pressed or Released. */
    int m_clicks; /*!< The number of clicks, whether is counts as a double click or not. */
    bool m_held; /*!< Whether the button is being held. */
    bool m_doubleClicked; /*!< Whether or not the event is a double click. */

    /*!
        * \brief Creates a Mouse Button Event structure.
        * \param button The button associated with the event.
        * \param state The state of the button, Pressed or Released.
        * \param clicks The number of clicks of associated button.
    */
    MouseButtonEvent(int button, int state, int clicks) {
        m_button = button;
        m_state = state;
        m_clicks = clicks;
        m_held = false;
        m_doubleClicked = false;
    }
};

/*!
    * \struct MouseMoveEvent "InputManager.h"
    * \brief Structure for holding all mouse movement data.
    *
    * Holds all necessary data for a mouse movement event, each time the user moves the mouse.
*/
struct ATOM_API MouseMoveEvent {
    bool m_hasMoved; /*!< Has the mouse moved or not. */
    double m_positionX; /*!< The X position of the mouse inside the window. */
    double m_positionY; /*!< The Y position of the mouse inside the window. */
    double m_relativeX; /*!< The relative motion of the mouse in the X axis. */
    double m_relativeY; /*!< The relative motion of the mouse in the Y axis. */

    MouseMoveEvent() {
        m_hasMoved = false;
    }

    /*!
        * \brief Creates a Mouse Move Event structure.
        * \param positionX The X position of the mouse.
        * \param positionY The Y position of the mouse.
        * \param relativeX The relative motion of mouse on X axis.
        * \param relativeY The relative motion of mouse on Y axis.
    */
    MouseMoveEvent(double positionX, double positionY, double relativeX, double relativeY) {
        m_positionX = positionX;
        m_positionY = positionY;
        m_relativeX = relativeX;
        m_relativeY = relativeY;
        m_hasMoved = true;
    }

};

/*!
    * \struct MouseScrollEvent "InputManager.h"
    * \brief Structure for holding all data for a mouse scroll event.
    *
    * Holds all necessary data for a mouse scroll event, each time the user scrolls the mouse wheel.
*/
struct ATOM_API MouseScrollEvent {
    int m_direction; /*!< Whether the direction of the mouse wheel is flipped. */
    bool m_hasMoved; /*!< Whether the mouse wheel has been scrolled or not. */
    double m_scrollAmountX; /*!< How far the mouse wheel has been scrolled in the X. */
    double m_scrollAmountY; /*!< How far the mouse wheel has been scrolled in the Y. */

    MouseScrollEvent() {
        m_hasMoved = false;
    }

    /*!
        * \brief Creates a Mouse scroll event structure.
        * \param direction Whether the direction of the wheel rotation is flipped or not.
        * \param scrollAmountX How far the mouse wheel has scrolled in the X direction.
        * \param scrollAmountY How far the mouse wheel has scrolled in the Y direction.
    */
    MouseScrollEvent(int direction, double scrollAmountX, double scrollAmountY) {
        m_direction = direction;
        m_scrollAmountX = scrollAmountX;
        m_scrollAmountY = scrollAmountY;
        m_hasMoved = true;
    }
};

class ATOM_API InputManager : public AlignedAllocation<BYTE16>
{
public:
    friend class Singleton<InputManager>;

    /*!
        * \brief Updates the InputManager
        *
        * This is where all the event polling is done, it is then stored into the relevant containers
        * for later retrieval so they can be processed by the user.
    */
    void Update();

    /*!
        * \brief Checks to see if a key is pressed.
        * \param key The key you are checking.
        * \return Whether the key has been pressed or not.
        *
        * Will check the keyboard events map, to see if the key specified has been
        * pressed or not.
    */
    bool IsKeyPressed(int key);

    /*!
        * \brief Checks to see if a key has been released.
        * \param key The key you are checking.
        * \return Whether the key has been released or not.
        *
        * Will check the keyboard events map to see if the key specified has been
        * released or not.
    */
    bool IsKeyReleased(int key);

    /*!
        * \brief Checks to see if a key is being held.
        * \param key The key you are checking.
        * \return Whether the key is being held or not.
        *
        * Will check the keyboard events map to see if a key is being held or not.
    */
    bool IsKeyHeld(int key);

    /*!
        * \brief Checks if a button has been clicked.
        * \param button The button you are checking.
        * \return Whether the button has been clicked or not.
        *
        * Will check the mouse button events map to see if the mouse button has been clicked or not.
    */
    bool IsButtonClicked(int button);

    /*!
        * \brief Checks if a button has been double clicked or not.
        * \param button The buttton you are checking.
        * \return Whether the button has been double clicked or not.
        *
        * Checks the mouse button events map to see if the button has been double clicked or not.
    */
    bool IsButtonDoubleClicked(int button);

    /*!
        * \brief Checks if a button has been released.
        * \param button The button you are checking.
        * \return Whether the button has been released or not.
        *
        * Checks the mouse button event map to see if the button has been released or not.
    */
    bool IsButtonReleased(int button);

    /*!
        * \brief Checks if a button is being held down.
        * \param button The button you are checking.
        * \return Whether the button is being held or not.
        *
        * Will check the mouse button event map to see if a button is being held or not.
    */
    bool IsButtonHeld(int button);

    /*!
        * \brief Checks if the mouse has moved.
        * \return Whether the mouse has moved or not.
    */
    bool HasMouseMoved();

    /*!
        * \brief Gets the mouse move event.
        * \return Returns a reference to the mouse movement structure containing all movement data.
    */
    const MouseMoveEvent& GetMouseMove();

    /*!
        * \brief Checks if the mouse has scrolled or not.
        * \return Whether the mouse wheel was scrolled or not.
    */
    bool HasScrolled();

    /*!
        * \brief Gets the Mouse scroll event.
        * \return Returns a reference to the mouse scroll event structure, containing all scroll data.
    */
    const MouseScrollEvent& GetMouseScroll();

    /*!
        * \brief Checks if the application has quit.
        * \return Whether the application or user has submitted a quit event.
    */
    bool HasQuit();

    /*!
        * \brief Requests that the application quits.
        *
        * This function adds a windows quit event into the window event map, which forces the application to quit.
    */
    void RequestQuit();

    /*!
        * \brief Checks for a windows event.
        * \param windowEvent The window event you are checking for.
        * \return Whether or not the window event exists or not.
    */
    bool CheckForWinEvent(int windowEvent);

    /*!
        * \brief Gets the window event data.
        * \param windowEvent The window event yoou want the data from.
        * \return The window event data that was requested.
    */
    WindowEvent GetWinEvent(int windowEvent);

    /*!
        * \brief Changes how mouse motion is handled
        * \param capture Whether the input system should change how mouse motion is handled.
        *
        * Changes how the input manager handles mouse motion. If the mouse is captured, then mouse motion
        * is relative motion. If it is not captured then it uses absolute positioning inside the window.
    */
    void CaptureMouse(bool capture);

private:
    /*!
        * \brief Adds a keyboard event to be processed.
        * \param keyCode The keycode of the key being added.
        * \param state The state of the Key, Whether is is Pressed or Released.
        * \param modifier The keycode of any modifier keys being pressed. eg SHIFT.
    */
    void AddKeyboardEvent(int keyCode, int state, int modifier);

    /*!
        * \brief Adds a window event to be processed.
        * \param windowID The ID of the window associated with the event.
        * \param type The type of event.
        * \param data1 The first piece of data for the event.
        * \param data2 The second piece of data for the event.
    */
    void AddWindowEvent(int windowID, int type, int data1, int data2);

    /*!
        * \brief Adds mouse scroll data.
        * \param scrollX The distance scrolled in the X direction.
        * \param scrollY The distance scrolled in the Y direction.
        * \param direction Whether or not the mouse wheel direction is flipped.
    */
    void AddMouseScrollEvent(double scrollX, double scrollY, int direction);

    /*!
        * \brief Adds mouse move data.
        * \param positionX The absolute position of the mouse on the X axis.
        * \param positionY The absolute position of the mouse on the Y axis.
        * \param relativeX The relative motion of the mouse on the X axis.
        * \param relativeY The relative motion of the mouse on the Y axis.
    */
    void AddMouseMoveEvent(double positionX, double positionY, double relativeX, double relativeY);

    /*!
        * \brief Adds a mouse button event.
        * \param button The mouse button associated with the event.
        * \param state The state of the button, whether it is Pressed or Released.
        * \param clicks How many times the button was clicked.
    */
    void AddMouseButtonEvent(int button, int state, int clicks);

    bool m_isMouseCaptured; /*!< Whether the mouse is captured or not. */
    std::map<int, KeyEvent> m_keyboardEvents; /*!< Map of Keyboard events. */
    std::map<int, bool> m_keysPressed; /*!< Map of Keyboard presses. */
    std::map<int, MouseButtonEvent> m_mouseButtonEvents; /*!< Map of Mouse button events. */
    std::map<int, WindowEvent> m_windowEvents; /*!< Map of window events. */

    MouseScrollEvent m_mouseScrollEvent; /*!< Mouse Scrolling structure to keep scroll data. */
    MouseMoveEvent m_mouseMoveEvent; /*!< Mouse Move structure to keep movement data. */
private:
    InputManager() {};
    InputManager(const InputManager&);
    //InputManager& operator=(const InputManager&) {};
};

template ATOM_API InputManager* Singleton<InputManager>::Instance();

typedef Singleton<InputManager> Input;