/*!
    * \class ThreadPool "ThreadPool.h"
    * \brief A class to handle multitasking within the game engine.
    *
    * Creates a collection of threads that continuously run concurrently, by taking "Jobs" from
    * a pool of jobs. Once all jobs are finished the threads go to sleep until needed again,
    * at which point they will wake back up and once again start completing jobs from the pool.
    * There are 3 job pools of varying priority; Low, Medium and High. High priority jobs will
    * always be completed first.
*/
#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include <queue>
#include <thread>
#include <future>
#include <vector>
#include <atomic>
#include <functional>
#include <condition_variable>

#include "Singleton.h"

/*!
    * \enum Job_Priority
    * Used to denote how important a job currently it.
*/
enum ATOM_API Job_Priority {
    LOW = 0,    /*!< Specifies a low priority job. */
    MEDIUM,     /*!< Specifies a medium priority job. */
    HIGH        /*!< Specifies a high priority job. */
};

typedef struct tagTHREADNAME_INFO
{
    DWORD dwType; // Must be 0x1000.
    LPCSTR szName; // Pointer to name (in user addr space).
    DWORD dwThreadID; // Thread ID (-1=caller thread).
    DWORD dwFlags; // Reserved for future use, must be zero.
} THREADNAME_INFO;

class ATOM_API ThreadPool
{
public:
    friend class Singleton<ThreadPool>;

    using Job = std::function<void()>;

    /*!
        * \brief Adds a "job" to the job pool.
        * \param job A job to add to the pool of jobs.
        * \return Returns the return value of the job that was executed.
        *
        * Adds a job to the pool of jobs, as a std::function object, so they can be passed in
        * as a lambda function. It utilizes varying return types by using a std::future so that
        * any return type may be used.
    */
    /*template<class T>
    auto AddJob(T job)->std::future<decltype(job())>;*/

    /*!
    * \brief Adds a "job" to the job pool.
    * \param job A job to add to the pool of jobs.
    * \param priority How urgent is this job.
    * \return Returns the return value of the job that was executed.
    *
    * Adds a job to the matching priority pool of jobs, as a std::function object, so they can be passed in
    * as a lambda function. It utilizes varying return types by using a std::future so that
    * any return type may be used.
    */
    template<class T>
    auto AddJob(T job, Job_Priority priority = Job_Priority::LOW)->std::future<decltype(job())>;
    
    /*!
        * \brief Creates the thread pool.
        * \param numThreads The number of threads to create for this pool.
        *
        * Creates a thread pool with a desired number of threads. This isn's the best practice method
        * as you may create more threads than your CPU can handle at once.
    */
    explicit ThreadPool(size_t numThreads);

    /*!
        * \brief Creates the thread pool.
        *
        * Creates the thread pool, but it will check to see how many hardware threads can run concurrently
        * on the executing CPU and will only make this number of threads for the pool. This is the safer option
        * for creating the thread pool.
    */
    explicit ThreadPool();

    ~ThreadPool();

    int m_numThreads;

private:
    /*!
        * \brief Starts the running of the pool.
        * \param numThreads The number of threads in the pool.
        *
        * Adds the threads to the pool, and starts there looping functtion so they can start
        * to complete jobs from the pools.
    */
    void StartPool(size_t numThreads);

    /*!
        * \brief Stops the thread pool.
        *
        * Sets the condition variable for all threads to true so that they are all stopped.
        * Then wakes up all threads and waits for their execution to finish before joining them
        * to the main thread.
    */
    void StopPool();

    /*!
        * \brief Names a chosen thread.
        * \param thread A pointer to the thread you wish to name.
        * \param threadName The string you wish to name the thread as.
        *
        * Names a chosen thread with the given string. This is primarily to help identify them
        * when profiling and debugging the application.
        * NOTE: This is a windows specific function.
    */
    void SetThreadName(std::thread* thread, const char* threadName)
    {
        DWORD threadId = ::GetThreadId(static_cast<HANDLE>(thread->native_handle()));
        SetThreadName(threadId, threadName);
    }


    void SetThreadName(uint32_t dwThreadID, const char* threadName)
    {
        THREADNAME_INFO info;
        info.dwType = 0x1000;
        info.szName = threadName;
        info.dwThreadID = dwThreadID;
        info.dwFlags = 0;

        __try
        {
            RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
        }
        __except (EXCEPTION_EXECUTE_HANDLER)
        {
        }
    }

    const DWORD MS_VC_EXCEPTION = 0x406D1388;

    bool m_isStopping;                          /*!< Are the threads currently stopping. */
    std::mutex m_poolMutex;                     /*!< The lock mutex for stopping race conditions. */
    std::condition_variable m_poolCondition;    /*!< Controls the threads sleeping or not. */
    std::vector<std::thread> m_threads;         /*!< Container of threads. */
    std::queue<Job> m_lowPriorityJobs;          /*!< Job Container for low priority jobs. */
    std::queue<Job> m_mediumPriorityJobs;       /*!< Job Container for medium priority jobs. */
    std::queue<Job> m_highPriorityJobs;         /*!< Job Container for high priority jobs. */
};

template ATOM_API ThreadPool* Singleton<ThreadPool>::Instance();

typedef Singleton<ThreadPool> JobSystem;

//template<class T>
//auto ThreadPool::AddJob(T job)->std::future<decltype(job())>
//{
//    auto wrapper = std::make_shared<std::packaged_task<decltype(job()) ()>>(std::move(job));
//    {
//        std::unique_lock<std::mutex> lock(m_poolMutex);
//        m_lowPriorityJobs.emplace([=] {
//            (*wrapper)();
//        });
//        m_poolCondition.notify_one();
//        return wrapper->get_future();
//    }
//}

template<class T>
auto ThreadPool::AddJob(T job, Job_Priority priority) -> std::future<decltype(job())>
{
    auto wrapper = std::make_shared<std::packaged_task<decltype(job()) ()>>(std::move(job));
    {
        std::unique_lock<std::mutex> lock(m_poolMutex);
        switch (priority) {
        case LOW:
            m_lowPriorityJobs.emplace([=] {
                (*wrapper)();
            });
            break;
        case MEDIUM:
            m_mediumPriorityJobs.emplace([=] {
                (*wrapper)();
            });
            break;
        case HIGH:
            m_highPriorityJobs.emplace([=] {
                (*wrapper)();
            });
            break;
        }
        m_poolCondition.notify_one();
        return wrapper->get_future();
    }
}

/*!
* \brief Checks to see if a job return value is ready.
* \param future A reference to the job future being checked.
* \return Whether the job return value is ready for retrieval or not.
*
* Takes the return future from a job assignment inside the ThreadPool. It will then check to see if
* the future return value is ready for retrieval or not. This was a job completing can be checked
* without making a thread wait on the value being returned.
*/
template<class T>
ATOM_API bool IsJobReady(std::future<T> const& future);

template<class T>
bool IsJobReady(std::future<T> const & future)
{
    //Make the future wait for a non-existent amount and check if the status is ready
    //If not ready then job has not completed yet.
    return future.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

template<class T>
ATOM_API bool AreJobsReady(std::vector<std::future<T>> const& futures);

template<class T>
bool AreJobsReady(std::vector<std::future<T>> const& futures)
{
    bool ready = true;
    for(auto& future : futures) {
        if(!IsJobReady(future)) {
            ready = false;
        }
    }
    return ready;
}

template<class T>
void WaitForJobCompletion(std::future<T> const & future){
    while(!IsJobReady(future)){}
}

template<class T>
void WaitForJobCompletion(std::vector<std::future<T>> const& futures) {
    while (!AreJobsReady(futures)) {}
}
