#include "ThreadPool.h"
#include <string>
#include <chrono>



ThreadPool::ThreadPool(size_t numThreads) :
    m_isStopping(false) //Sets initial values.
{
    //Starts the thread pool with the number of threads decided.
    StartPool(numThreads);
}


ThreadPool::ThreadPool() :
    m_isStopping(false)
{
    //Queries the pc to see how many threads can run on the hardware at once.
    size_t maxNumThreads = std::thread::hardware_concurrency();
    StartPool(maxNumThreads);
}

ThreadPool::~ThreadPool()
{
    //Upon destruction stops all threads and deletes them all.
    StopPool();
}

void ThreadPool::StartPool(size_t numThreads)
{
    for (size_t i = 0; i < numThreads; i++) {
        //For each thread it will place it into the vector and give it the following function using a lambda.
        std::thread worker([=] {
            while (true) { //Loop forever inside the thread.
                Job job;
                {
                    //Lock the mutex, making the thread pool variables race safe.
                    std::unique_lock<std::mutex> lock(m_poolMutex);
                    //Set each thread to be waiting on the conditional variable. Will make thread wait until either hasStopped is true or until
                    //the job queues are not empty.
                    m_poolCondition.wait(lock, [=] { return m_isStopping || !m_lowPriorityJobs.empty() || !m_mediumPriorityJobs.empty() || !m_highPriorityJobs.empty(); });
                    //If has stopped is true then break out of infinite loop stopping thread execution.
                    if (m_isStopping) {
                        break;
                    }
                    //If not stopping then grab a job from the queue
                    if (!m_highPriorityJobs.empty()) {
                        job = std::move(m_highPriorityJobs.front());
                        m_highPriorityJobs.pop();
                    }
                    if (!m_mediumPriorityJobs.empty()) {
                        job = std::move(m_mediumPriorityJobs.front());
                        m_mediumPriorityJobs.pop();
                    }
                    if (!m_lowPriorityJobs.empty()) {
                        job = std::move(m_lowPriorityJobs.front());
                        m_lowPriorityJobs.pop();
                    }
                }
                job();
            }
        });
        SetThreadName(&worker, ("Pool Thread " + std::to_string(i)).c_str());
        m_threads.push_back(std::move(worker));
    }
}

void ThreadPool::StopPool()
{
    {
        std::unique_lock<std::mutex> lock(m_poolMutex);
        m_isStopping = true;
    }

    m_poolCondition.notify_all();

    for (auto &thread : m_threads) {
        thread.join();
    }
}


