#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
class MessageQueue {
public:
    void push(const T& msg) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            q.push(msg);
        }
        cv.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this]{ return !q.empty(); });
        T msg = q.front();
        q.pop();
        return msg;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mtx);
        return q.empty();
    }

private:
    mutable std::mutex mtx;
    std::queue<T> q;
    std::condition_variable cv;
};
