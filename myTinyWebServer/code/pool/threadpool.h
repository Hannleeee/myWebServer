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
                // unique_lock默认对传入的mutex加锁
                std::unique_lock<std::mutex> locker(pool->mtx);
                // 线程反复检查pool中task情况
                while (true) {
                    if (!pool->tasks.empty()) {
                        auto task = std::move(pool->tasks.front());
                        pool->tasks.pop();
                        locker.unlock();
                        task();
                        locker.lock();
                    }
                    else if (pool->isClosed) break;
                    else pool->cond.wait(locker);
                }
            }).detach();
        }
    }

    ThreadPool(ThreadPool &&) = default;

    ~ThreadPool() {
        // 检查_pool是否为空指针，即_pool指向对象是否已被销毁
        if (static_cast<bool>(_pool)) {
            // 使用{}限制lock_guard范围
            {
                std::lock_guard<std::mutex> locker(_pool->mtx);
                _pool->isClosed = true;
            }
            _pool->cond.notify_all();
        }
    }

    template<typename T>
    void AddTask(T &&task) {
        {
            std::lock_guard<std::mutex> locker(_pool->mtx);
            _pool->tasks.emplace(std::forward<T>(task));
        }
        // 随即唤醒一个线程完成工作
        _pool->cond.notify_one();
    }

private:
    struct Pool {
        std::mutex mtx;
        std::condition_variable cond;
        bool isClosed;
        // 为什么用std::function?有啥好处？
        std::queue<std::function<void()>> tasks;
    };
    std::shared_ptr<Pool> _pool;
};

#endif