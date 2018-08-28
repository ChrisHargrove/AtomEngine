/*!
    * \class IOManager "IOManager.h"
    * \brief Manager to handle all file loading and saving within the engine.
    *
    * This class is designed to allow for any data to be loaded or saved from files of varying type.
    * This is achieved using the Cereal Serialization Library.
*/
#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include "Singleton.h"
#include "AlignedAllocation.h"

#include <string>
#include <fstream>
#include <CEREAL/archives/binary.hpp>
#include <CEREAL/archives/json.hpp>
#include <CEREAL/archives/xml.hpp>

enum SerializationType {
    BINARY = 0,
    JSON,
    XML
};

class ATOM_API IOManager : public AlignedAllocation<BYTE16>
{
public:
    friend class Singleton<IOManager>;

    bool OpenFileInput(const std::string& fileName, SerializationType type);
    bool OpenFileOutput(const std::string& fileName, SerializationType type);

    template<typename T>
    bool Write(T data);

    template<typename T, typename... Args>
    bool Write(T data, Args... args);

    bool CloseFile();

private:
    std::fstream m_fileStream;
    SerializationType m_type;

    cereal::XMLInputArchive* m_inputXML;
    cereal::XMLOutputArchive* m_outputXML;
    cereal::JSONInputArchive* m_inputJSON;
    cereal::JSONOutputArchive* m_outputJSON;
    cereal::BinaryInputArchive* m_inputBinary;
    cereal::BinaryOutputArchive* m_outputBinary;

    IOManager();
    ~IOManager();
};

template<typename T>
inline bool IOManager::Write(T data)
{
    switch (m_type) {
    case BINARY:
        break;
    case XML:
        break;
    case JSON:
        break;
    }
    return false;
}

template<typename T, typename... Args>
inline bool IOManager::Write(T data, Args... args)
{
    Write(args...);
    Write(data);

    return false;
}

