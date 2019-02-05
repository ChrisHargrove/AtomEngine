/*!
    * \class Timer "Timing.h"
    * \brief A Timer for getting Delta Time
    *
    * This class is designed for use during the main program loop, to get the elapsed time
    * each frame, and return it for use by the main program.
*/

#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include <chrono>

class ATOM_API Timer
{
public:
    Timer();
    ~Timer();

    /*!
        * \brief Will start the timer.
        *
        * This function will start the timer and get the starting time, this function is only
        * called once before entering into the main game loop. It should not be called again.
    */
    void Start();

    /*!
        * \brief Will stop the timer.
    */
    void End();

    /*!
        * \brief Gets the elapsed time since last call.
        * \return The time elapsed since last frame in seconds.
        *
        * This function calculates the time elapsed since the last frame and also resets the 
        * current time points so they can be used again next frame.
    */
    float GetDelta();

private:
    std::chrono::duration<float> m_elapsedTime; /*!< The time that has passed since last frame. */
    std::chrono::high_resolution_clock::time_point m_startTime; /*!< The time at the start of the frame. */
    std::chrono::high_resolution_clock::time_point m_endTime; /*!< The time at the end of the frame. */
};


