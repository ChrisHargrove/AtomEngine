#pragma once

#include <mutex>
#include <map>
#include <deque>
#include <fstream>
#include "Singleton.h"
#include "AlignedAllocation.h"

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

class ATOM_API ProfilerManager : public AlignedAllocation<BYTE16>
{
public:
    friend class Singleton<ProfilerManager>;

    void Initialize(const std::string& fileName);
    void Shutdown();

    void Start(const std::string& label);
    void End(const std::string& label);

private:
    std::ofstream* m_fileStream;
    std::mutex m_profilerLock;

    std::map<std::string,std::chrono::high_resolution_clock::time_point> m_startTimes;
    std::map<std::string, std::deque<float>> m_averageTimes;

private:
    ProfilerManager() {};
    ProfilerManager(const ProfilerManager&);
};

template ATOM_API ProfilerManager* Singleton<ProfilerManager>::Instance();

typedef Singleton<ProfilerManager> Profiler;