#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include "AlignedAllocation.h"
#include "Singleton.h"

#include <string>
#include <fstream>

class ATOM_API IOManager : public AlignedAllocation<BYTE16>
{
public:
    friend class Singleton<IOManager>;

    bool Open(const std::string& fileName, unsigned int mode);
    void Close();

    template<class Archive, class T>
    void Serialize(T& t);

private:

    IOManager() {};
    IOManager(const IOManager&);
    ~IOManager() {};

    std::fstream m_fileStream;
};

template ATOM_API IOManager* Singleton<IOManager>::Instance();

typedef Singleton<IOManager> IO;

template<class Archive, class T>
inline void IOManager::Serialize(T& t)
{
    Archive archive(m_fileStream);
    archive(t);
}


