#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>

template<typename T>
class MessageQueue {
public:
    void push(const T& msg) {
        {
            std::lock_guard<std::mutex> lock(mtx_);
            if (closed_) return;
            q_.push(msg);
        }
        cv_.notify_one();
    }

    std::optional<T> pop() {
        std::unique_lock<std::mutex> lock(mtx_);
        cv_.wait(lock, [&]{ return closed_ || !q_.empty(); });
        if (q_.empty()) return std::nullopt; 
        T v = q_.front(); q_.pop();
        return v;
    }

    std::optional<T> try_pop() {
        std::lock_guard<std::mutex> lock(mtx_);
        if (q_.empty()) return std::nullopt;
        T v = q_.front(); q_.pop();
        return v;
    }

    void close() {
        {
            std::lock_guard<std::mutex> lock(mtx_);
            closed_ = true;
        }
        cv_.notify_all();
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mtx_);
        return q_.empty();
    }

private:
    mutable std::mutex mtx_;
    std::condition_variable cv_;
    std::queue<T> q_;
    bool closed_ = false;
};
