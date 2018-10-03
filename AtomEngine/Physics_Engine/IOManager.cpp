#include "IOManager.h"
#include "LogManager.h"


bool IOManager::Open(const std::string& fileName, unsigned int mode)
{
    m_fileStream.open(fileName.c_str(), mode);

    if (!m_fileStream.is_open()) {
        Logger::Instance()->LogError("[IOManager] Failed To Open File: " + fileName + " during Serialization.");
        return false;
    }

    return true;
}

void IOManager::Close()
{
    if (!m_fileStream.is_open()) {
        Logger::Instance()->LogWarning("[IOManager] There was no file to close after serialization. Check this...");
        return;
    }
    m_fileStream.close();
}
