/* 
* 通用线程池
*/
#pragma once

#include <mutex>
#include <queue>
#include <vector>
#include <functional>
#include <thread>
#include <future>

class ThreadPool {
public:
    // 初始化线程池
    void Init(int pool_size);

    // 停止线程池
    void Stop();

    // 向线程池添加任务，对外呈现形式，obj.Exec(taskfunc, para1, para2);
    template <typename FuncType, typename ... Args>
    auto Exec(FuncType&& func, Args&& ... args) -> std::future<decltype(func(args...))> {
        //
        //using RetType = decltype(func(args...));
        //类型能推断
        auto task = std::packaged_task<std::function<void()>>(std::bind(std::forward<FuncType>(func), std::forward<Args>(args)...));
        auto ret = task.get_future();

        std::unique_lock<std::mutex> lock(m_mutex);
        m_tasks.emplace(std::move(task));

        lock.unlock();
        m_cv.notify_one();

        return ret;
    }

private:
    // 工作线程运行任务
    void Run();

    // 任务队列
    std::queue<std::function<void()>> m_tasks;

    // 工作线程
    std::vector<std::shared_ptr<std::thread>> m_workers;

    // 互斥量，对m_tasks,m_workers加锁
    std::mutex m_mutex;

    // 条件变量
    std::condition_variable m_cv;

    // 设置线程池是否结束
    bool m_isStop = false;

};