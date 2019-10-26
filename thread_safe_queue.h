#ifndef MULTI_THREAD_QUEUE_H_
#define MULTI_THREAD_QUEUE_H_

#include <queue>
#include <utility>
#include <mutex>

template <typename T>
class ThreadSafeQueue {
 public:
    ThreadSafeQueue() = default;
    ~ThreadSafeQueue() = default;
    ThreadSafeQueue(const ThreadSafeQueue& queue) = delete;

    void push(T&& t) {
        std::lock_guard<std::mutex> lock(m_);
        queue_.push(std::move(t));
    }

    bool pop(T& t) {
        std::lock_guard<std::mutex> lock(m_);
        if (queue_.empty()) {
            return false;
        }
        t = std::move(queue_.front());
        queue_.pop();
        return true;
    }

    bool empty() {
        std::lock_guard<std::mutex> lock(m_);
        return queue_.empty();
    }


 private:
    std::queue<T> queue_;
    std::mutex m_;
};

#endif  // MULTI_THREAD_QUEUE_H_
