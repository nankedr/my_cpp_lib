#include "simple_thread_pool.h"

void ThreadPool::Init(int pool_size) {
    m_workers.reserve(pool_size);
    for (int i = 0; i < pool_size; ++i) {
        m_workers.push_back(std::make_shared<std::thread>(&ThreadPool::Run, this));
    }
}

void ThreadPool::Stop() {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_isStop = true;
    lock.unlock();

    m_cv.notify_all();

    for (auto& work : m_workers) {
        if (work->joinable()) {
            work->join();
        }
        work = nullptr;
    }
}

/*
*/

void ThreadPool::Run() {
    while (!m_isStop) {
        std::unique_lock<std::mutex> lock(m_mutex);

        if (m_tasks.empty()) {
            m_cv.wait(lock, []() {return m_isStop || !m_tasks.empty(); });
        }

        if (m_isStop) {
            break;
        }

        //
        auto task = m_tasks.front();
        m_tasks.pop();

        try {
            task();
        }
        catch (...) {

        }
        
    }
}





