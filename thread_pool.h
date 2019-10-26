#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include "thread_safe_queue.h"

#include <cstddef>
#include <future>
#include <condition_variable>

class ThreadPool {
 public:
    explicit ThreadPool(size_t pool_size);
    ThreadPool() = delete;
    ThreadPool(const ThreadPool& thread_pool) = delete;
    ~ThreadPool();

    template<class F, class... Args>
    auto submit(F&& f, Args&&... args) -> decltype(std::future<F>(f));
    void stop();

 private:
    ThreadSafeQueue<std::function<void()>> queue_;
    std::mutex work_m_;
    std::condition_variable work_cv_;
    bool running_;
    size_t pool_size_;
};

#endif
