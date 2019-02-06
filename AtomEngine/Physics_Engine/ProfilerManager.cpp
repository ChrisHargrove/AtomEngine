#include "ProfilerManager.h"
#include <string>
#include <chrono>


void ProfilerManager::Initialize(const std::string & fileName)
{
    m_fileStream = new std::ofstream(fileName);
}

void ProfilerManager::Shutdown()
{
    delete m_fileStream;
}

void ProfilerManager::Start(const std::string& label)
{
    //add label to the queue
    m_labelOrder.push_back(label);
    //store the start time of the profile, and use a key value of its label.
    m_profiles.emplace(std::make_pair(label, std::chrono::high_resolution_clock::now()));
}

void ProfilerManager::End()
{
    //sort out labels
    std::string label = m_labelOrder.back();
    std::string prefix = "[PROFILER]";

    //calculate time taken for this label

    std::string timeTaken = std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - m_profiles.at(label)).count());

    //output it to the file
    (*m_fileStream) << prefix << label << " took " << timeTaken << "ms to complete\n";

    //remove from the list of profiles and labels.
    m_labelOrder.pop_back();
    m_profiles.erase(label);
}
