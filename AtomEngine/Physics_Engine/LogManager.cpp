#include "LogManager.h"
#include <Windows.h>

void LogManager::Initialize(const std::string & fileName)
{
    m_fileStream = new std::ofstream(fileName);
}

void LogManager::Shutdown()
{
    delete m_fileStream;
}

void LogManager::LogInfo(const std::string & message)
{
    std::unique_lock<std::mutex> lock(m_LogLock);
#ifdef DEBUG
    //Sets console writing colour to Grey
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
#endif
    LogMessage("[INFO]", message);
}

void LogManager::LogDebug(const std::string & message)
{
    std::unique_lock<std::mutex> lock(m_LogLock);
#ifdef DEBUG
    //Sets console writing colour to Green
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
#endif
    LogMessage("[DEBUG]", message);
}

void LogManager::LogWarning(const std::string & message)
{
    std::unique_lock<std::mutex> lock(m_LogLock);
#ifdef DEBUG
    //Sets console writing colour to Yellow
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
#endif
    LogMessage("[WARNING]", message);
}

void LogManager::LogError(const std::string & message)
{
    std::unique_lock<std::mutex> lock(m_LogLock);
#ifdef DEBUG
    //Sets console writing colour to Red
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
#endif
    LogMessage("[ERROR]", message);
}

const std::string LogManager::GetCurrTime()
{
    std::string result = "";
    //Get the current time
    time_t currentTime = time(0);
    //If the current time is less than 0 return empty string.
    if (currentTime < 0) {
        return result;
    }
    //Windows timepoint
    tm timePoint;
    //Place the current time into the timepoint.
    localtime_s(&timePoint, &currentTime);

    const int stringTimeLength = 10;
    //Character array to store the formatted time data inside
    char stringTime[stringTimeLength];

    //Adds the formatted time string to the return string.
    if (strftime(stringTime, stringTimeLength, "%H:%M:%S", &timePoint) > 0) {
        result.append(stringTime);
    }
    //Returns the time as a string.
    return result;
}

void LogManager::LogMessage(const std::string & prefix, const std::string & message)
{
    //Outputs the message to the filestream directly to the file connected to the filestream.
    (*m_fileStream) << '[' << GetCurrTime() << ']' << prefix << ' ' << message << std::endl;
#ifdef DEBUG
    //Outputs the message to the console window.
    std::cout << '[' << GetCurrTime() << ']' << prefix << ' ' << message << std::endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
#endif
}

