/*!
    * \class LogManager "LogManager.h"
    * \brief Class to manage logging of errors and debug information.
    *
    * This is a class to deal with all logging, it will output the log to a .log file.
    * Will also output to the console screen during debug, and will show the different message types
    * in different colours.
*/
#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include <ctime>
#include <mutex>
#include <string>
#include <fstream>
#include <iostream>
#include "Singleton.h"
#include "AlignedAllocation.h"

class ATOM_API LogManager : public AlignedAllocation<BYTE16>
{
public:
    friend class Singleton<LogManager>;

    /*!
        * \brief Initializes Log Manager to write to the file specified.
        * \param fileName The name of the file that you wish to write the log to.
    */
    void Initialize(const std::string& fileName);

    /*!
        * \brief Shuts down the Log Manager and closes the filestream.
    */
    void Shutdown();

    /*!
        * \brief Log a message, with a [INFO] prefix.
        * \param message The message that you want to log.
    */
    void LogInfo(const std::string& message);

    /*!
        * \brief Log a message, with a [DEBUG] prefix.
        * \param message The message that you want to log.
    */
    void LogDebug(const std::string& message);

    /*!
        * \brief Log a message, with a [WARNING] prefix.
        * \param message The message that you want to log.
    */
    void LogWarning(const std::string& message);

    /*!
        * \brief Log a message, with a [ERROR] prefix.
        * \param message The message that you want to log.
    */
    void LogError(const std::string& message);

private:
    /*!
        * \brief Gets the current time.
        * \return Returns the current time as a string.
    */
    const std::string GetCurrTime();

    /*!
        * \brief Log a message, with a specified prefix.
        * \param prefix The prefix to add to the message.
        * \param message The message that you want to log.
    */
    void LogMessage(const std::string& prefix, const std::string& message);

    std::ofstream* m_fileStream; /*!< Log Manager's Filestream. */

    std::mutex m_LogLock;   /*!< Mutex for locking iostream output so there is no interleaving messages. */

private:
    LogManager() {};
    LogManager(const LogManager&);
   // LogManager& operator=(const LogManager&) {}

};

template ATOM_API LogManager* Singleton<LogManager>::Instance();

typedef Singleton<LogManager> Logger;