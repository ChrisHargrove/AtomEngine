#include "Timing.h"



Timer::Timer()
{
}


Timer::~Timer()
{
}

void Timer::Start()
{
    m_startTime = std::chrono::high_resolution_clock::now();
}

void Timer::End()
{
    m_endTime = std::chrono::high_resolution_clock::now();
}

float Timer::GetDelta()
{
    End();
    m_elapsedTime = std::chrono::duration_cast<std::chrono::duration<float>>(m_endTime - m_startTime);
    Start();

    return m_elapsedTime.count();
}
