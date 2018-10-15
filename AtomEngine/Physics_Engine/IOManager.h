/*!
    * \class IOManager "IOManager.h"
    * \brief Manager for handing all input output operation , such as serialization
    * 
    * This class will handle all the serialization for the game engine. Can output data to different formats.
    * Not to be used for basic file reading.
*/
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
#include <CEREAL/cereal.hpp>

class ATOM_API IOManager : public AlignedAllocation<BYTE16>
{
public:
    friend class Singleton<IOManager>;

    bool Open(const std::string& fileName, unsigned int mode);
    void Close();

    template<class Archive, class T>
    void Serialize(T& t);

    template<class Archive, class T>
    void Serialize(T& t, const std::string& name);

private:

    IOManager() {};
    IOManager(const IOManager&) {};
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

template <class Archive, class T>
inline void IOManager::Serialize(T& t, const std::string& name)
{
    Archive archive(m_fileStream);
    archive(cereal::make_nvp(name, t));
}


