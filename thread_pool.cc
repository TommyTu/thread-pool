#include "thread_pool.h"

#include <future>
#include <utility>

ThreadPool::ThreadPool(size_t pool_size) {
    this->pool_size_ = pool_size;
    this->running_ = true;
    for (int i = 0; i < pool_size; i++) {
        std::thread([this](){
            std::function<void()> func;
            while (true) {
                {
                    std::unique_lock<std::mutex> lock(this->work_m_);
                    if (this->queue_.empty()) {
                        this->work_cv_.wait(lock);
                    }
                    if (!this->running_) {
                        break;
                    }
                    if (!this->queue_.pop(func)) {
                        continue;
                    }
                }
                func();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    stop();
}

template<class F, class... Args>
auto ThreadPool::submit(F&& f, Args&&... args) -> decltype(std::future<F>(f)) {
    auto func = std::bind(std::forward<F>(f), std::forward<Args...>(args...));
    std::packaged_task<void()> task(func);
    std::function<void()> wrap_func = [&task](){
        task();
    };
    this->queue_.push(std::move(wrap_func));
    this->work_cv_.notify_one();

    return task.get_future();
}

void ThreadPool::stop() {
    running_ = false;
    this->work_cv_.notify_all();
}
