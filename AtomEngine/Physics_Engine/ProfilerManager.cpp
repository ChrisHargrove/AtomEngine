#include "ProfilerManager.h"
#include <string>
#include <chrono>


void ProfilerManager::Initialize(const std::string & fileName)
{
    m_fileStream = new std::ofstream(fileName);
}

void ProfilerManager::Shutdown()
{
    for(auto label : m_averageTimes)
    {
        float totalTime = 0;
        for(auto time : label.second)
        {
            totalTime += time;
        }
        float averageTime = totalTime / label.second.size();

        (*m_fileStream) << '[' << label.first << ']' << " Took Average of: " << std::to_string(averageTime * 1000) + "ms" << std::endl;
    }

    delete m_fileStream;
}

void ProfilerManager::Start(const std::string& label)
{
    m_startTimes[label] = std::chrono::high_resolution_clock::now();
}

void ProfilerManager::End(const std::string& label)
{
    auto endTime = std::chrono::high_resolution_clock::now();
    auto startTime = m_startTimes[label];

    auto timeTaken = std::chrono::duration_cast<std::chrono::duration<float>>(endTime - startTime).count();

    if(m_averageTimes[label].size() < 10)
    {
        m_averageTimes[label].push_back(timeTaken);
    }
    else
    {
        m_averageTimes[label].pop_front();
        m_averageTimes[label].push_back(timeTaken);
    }

}
