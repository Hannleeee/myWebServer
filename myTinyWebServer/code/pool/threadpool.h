#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>
#include <functional>

class ThreadPool {
public:
    // 注意只有当类没有声明任何构造函数时，才会自动生成默认构造函数
    // 因此需要使用default要求编译器生成默认构造函数
    ThreadPool() = default;

    explicit ThreadPool(size_t threadCount = 8) : _pool(std::make_shared<Pool>()) {
        assert(threadCount > 0);
        for (size_t i = 0; i < threadCount; ++i) {
            // 传入lambda函数
            std::thread([pool = _pool] {
                std::unique_lock<std::mutex> locker(pool->mtx);
            })
        }
    }

private:
    struct Pool {
        std::mutex mtx;
        std::condition_variable cond;
        bool isClosed;
        std::queue<std::function<void()>> tasks;
    };
    std::shared_ptr<Pool> _pool;
};

#endif