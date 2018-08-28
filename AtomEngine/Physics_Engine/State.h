/*!
    * \class State "State.h"
    * \brief Abstract Base Class for all engine states.
*/
#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include "AlignedAllocation.h"

class ATOM_API State : public AlignedAllocation<BYTE16>
{
public:
    /*!
        * \brief Pure Virtual function used to initialize the game state.
        * \return Whether the state was successfully initialized or not.
        *
        * Must be overridden in each game state as this is the primary function which handles
        * all the loading and setting up the initial state of each Game State.
    */
    virtual bool Initialize() = 0;

    /*!
        * \brief Pure Virtual function used to handle all input for the state.
        *
        * Must be overriden in each game state as this is where all user input will be processed
        * for each game state.
    */
    virtual void Input() = 0;

    /*!
        * \brief Pure Virtual function used to handle all updating for the state.
        * \param delta The time elapsed since last frame.
        * 
        * Must be overriden in each game state as this is where all user updates will be processed
        * for each game state.
    */
    virtual void Update(float delta) = 0;

    /*!
        * \brief Pure Virtual function used to handle all rendering for the state.
        *
        * Must be overriden in each game state as this is where all rendering takes place.
    */
    virtual void Render() = 0;

    /*!
        * \brief Pure Virtual function which will shutdown the game state.
        * \return Whether the state shutdown successfully or not.
        *
        * Must be overriden in each game state. This function is where all memory allocation will be freed.
        * And all end of state actions will be performed.
    */
    virtual bool Shutdown() = 0;

    /*!
        * \brief Pure Virtual function used to pause the game state.
        *
        * Must be overriden in each game state, this is where the user will handle all their pause conditions.
    */
    virtual void Pause() = 0;

    /*!
        * \brief Pure Virtual function used to resume the game state.
        *
        * Must be overriden in each game state as this is where all use will handle all their resume conditions.
    */
    virtual void Resume() = 0;

    /*!
        * \brief Pure Virtual function to get the whether the current state is paused or not.
        * \return Whether the state is paused or not.
        * 
        * Must be overriden in each game state, this is where the user can implement extra checks to decide if the
        * current state is paused or not.
    */
    virtual bool IsPaused() = 0;

protected:
    bool m_isPaused; /*!< Whether this state is paused or not. */

};