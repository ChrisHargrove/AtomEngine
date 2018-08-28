/*!
    * \class StateManager "StateManager.h"
    * \brief Manager for handling the Finite State Machine.
    *
    * Handles all the state changes and flow of the Finite State Machine, and
    * allows for the control of states using Strings rather than each state having to
    * know about other states.
*/
#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include <map>
#include <stack>
#include "State.h"
#include "LogManager.h"
#include "Singleton.h"
#include "AlignedAllocation.h"

class ATOM_API StateManager : public AlignedAllocation<BYTE16>
{
public:
    friend class Singleton<StateManager>;

    /*!
        * \brief Calls the Input() function for the currently active state.
        *
        * Will get the currently active state and call its Input() function using polymorphism.
    */
    void Input();

    /*!
        * \brief Calls the Update(float delta) function for the currently active state.
        * \param delta The time elapsed since the last frame.
        *
        * Will get the currently active state and call its Update(delta) function using polymorphism. 
        * So that all game updates stay the same speed regardless of machine, the elapsed time is passed
        * to this function to allow for scaling due to CPU speed.
    */
    void Update(float delta);

    /*!
        * \brief Calls the Render() function for the currently active state.
        *
        * Will get the currently active state and call its Render() function using polymorphism.
    */
    void Render();

    /*!
        * \brief Calls the Shutdown() function for the currently active state.
        *
        * Will get the currently active state and call its Shutdown() function using polymorphism.
    */
    bool Shutdown();

    /*!
        * \brief Will change the currently active state to the one specified.
        * \param stateName The name of the state to change to.
        * \return Whether the state change was successful or not.
        *
        * Will search the active stack to see if the state is already there, if not it will then
        * push this state onto the stack as the currently active state.
    */
    bool ChangeState(const std::string& stateName);

    /*!
        * \brief Will add a new state onto the active stack, and making it currently active.
        * \param stateName The name of the state to push onto the stack.
        * \param search Whether to search the state list for state, to see if it exists. Default to true;
        * return Whether the state push was successful or not.
        *
        * Will push the new state specified onto the stack and make it the currently active stack.
    */
    bool PushState(const std::string& stateName, bool search = true);

    /*!
        * \brief Will pop the currently active state off the stack.
        * \return Whether the pop was successful or not.
        *
        * Will pop the currently active state off the top of the stack and will make the next state resume.
    */
    bool PopState();

    /*!
        * \brief Will add a state to the state list.
        * \param stateName The name of the state, which is also its key.
        * \param state A pointer to the state being added.
        * \return Whether the state was added successfully or not.
        *
        * Will add the state pointer to a list of states that can then be accessed and used using
        * their key value.
    */
    bool AddState(const std::string& stateName, State* state);

    /*!
        * \brief Will get a pointer to the state specified.
        * \param stateName The name of the state yoou are looking for.
        * \return A pointer to the state asked for, will return nullptr if state is not found.
        *
        * Searches the state list and looks for the state the user specified, if it cannot find the state
        * it will return a nullptr.
    */
    State* GetState(const std::string& stateName);

private:
    std::stack<State*> m_activeStack;   /*!< The active stack of states. Can only access the top at any time. */
    std::map<std::string, State*> m_stateList; /*!< List of all states, mapped using a string key value. */

private:
    StateManager() {};
    StateManager(const StateManager&);
   // StateManager& operator=(const StateManager&) {};
};

template ATOM_API StateManager* Singleton<StateManager>::Instance();

typedef Singleton<StateManager> StateMachine;