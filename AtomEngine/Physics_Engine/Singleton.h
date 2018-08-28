/*!
    * \class SingletonLock "Singleton.h"
    * \brief Class that makes Singleton classes thread safe.
    *
    * This class primarily aims to make Singleton classes thread safe by using windows built in Critical Sections.
    * By doing this manager classes can be used across threads safely. This class is also 16 byte aligned using the 
    * Aligned Allocation.
*/

#pragma once

#define WIN32_LEAN_AND_MEAN

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include <memory>
#include <Windows.h>
#include "AlignedAllocation.h"

class ATOM_API SingletonLock : public CRITICAL_SECTION, public AlignedAllocation<BYTE16>
{
public:
    /*!
        * Makes the constructor use windows InitializeCriticalSection function.
    */
    SingletonLock() { InitializeCriticalSection(this); }

    /*!
    * Makes the destructor use windows DeleteCriticalSection function.
    */
    ~SingletonLock() { DeleteCriticalSection(this); }

private:
    SingletonLock(SingletonLock const&) = delete;
    SingletonLock& operator=(SingletonLock const&) = delete;
};


/*!
    * \class Singleton "Singleton.h"
    * \brief Class that makes Singleton classes.
    *
    * This class makes any other class a singleton through inheritance. It will also hide away all member functions of the
    * singleton class, until you specifically access the instance of the singleton class.
*/

template<class T>
class ATOM_API Singleton : public AlignedAllocation<BYTE16>
{
public:
    /*!
        * \brief Gives Access to the singleton instance to call member functions.
    */
    inline static T* Instance();

private:
    Singleton() = default;
    ~Singleton() = default;
    Singleton(Singleton const &) = delete;
    Singleton& operator=(Singleton const&) = delete;

    static T* s_instance;
    static SingletonLock s_singletonLock; /*!< Static Singleton Instance*/
};

template<class T> SingletonLock Singleton<T>::s_singletonLock;

template<class T> T* Singleton<T>::s_instance = nullptr;

template<class T>
inline T* Singleton<T>::Instance()
{
    //Ensures thread safe entry into singleton instance.
    EnterCriticalSection(&s_singletonLock);
    //If not already existing, then create a pointer to the singleton instance.
    if (s_instance == nullptr) {
        s_instance = new T();
    }
    //Leave the singleton instance safely.
    LeaveCriticalSection(&s_singletonLock);
    //Return the pointer to the singleton instance.
    return s_instance;
}