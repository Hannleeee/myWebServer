#ifndef BLOCKQUEUE_H
#define BLOCKQUEUE_H

#include <mutex>
#include <deque>
#include <condition_variable>
#include <sys/time.h>

template<typename T>
class BlockDeque {
public:
    explicit BlockDeque(size_t maxCapacity = 1000);
    ~BlockDeque();

    void Clear();
    bool Empty();
    bool Full();
    void Close();
    size_t Size();
    size_t Capacity();
    T Front();
    T Back();
    void PushBack(const T &item);
    void PushFront(const T &item);
    bool Pop(T &item);
    bool Pop(T &item, int timeout);
    void Flush();

private:
    std::deque<T> _deq;
    size_t _capacity;
    std::mutex _mtx;
    bool _isClose;
    std::condition_variable _condConsumer;
    std::condition_variable _condProducer;
};

template<typename T>
BlockDeque<T>::BlockDeque(size_t maxCapacity) : _capacity(maxCapacity) {
    assert(maxCapacity > 0);
    _isClose = false;
}

template<typename T>
void BlockDeque<T>::Close() {
    {
        std::lock_guard<std::mutex> locker(_mtx);
        // 关闭前先清空_deq
        _deq.clear();
        _isClose = true;
    }
    _condProducer.notify_all();
    _condConsumer.notify_all();
}

// 干啥用的？
template<typename T>
void BlockDeque<T>::Flush() {
    _condConsumer.notify_one();
}

template<typename T>
void BlockDeque<T>::Clear() {
    std::lock_guard<std::mutex> locker(_mtx);
    _deq.clear();
}

template<typename T>
T BlockDeque<T>::Front() {
    std::lock_guard<std::mutex> locker(_mtx);
    _deq.clear();
}

template<typename T>
T BlockDeque<T>::Back() {
    std::lock_guard<std::mutex> locker(_mtx);
    return _deq.back();
}

template<typename T>
size_t BlockDeque<T>::Size() {
    std::lock_guard<std::mutex> locker(_mtx);
    return _deq.size();
}

template<typename T>
size_t BlockDeque<T>::Capacity() {
    std::lock_guard<std::mutex> locker(_mtx);
    return _capacity;
}

template<typename T>
void BlockDeque<T>::PushBack(const T &item) {
    std::unique_lock<std::mutex> locker(_mtx);
    while (_deq.size() >= _capacity) _condProducer.wait(locker);
    _deq.push_back(item);
    _condConsumer.notify_one();
}

template<typename T>
void BlockDeque<T>::PushFront(const T &item) {
    std::unique_lock<std::mutex> locker(_mtx);
    while (_deq.size() >= _capacity) _condProducer.wait(locker);
    _deq.push_front(item);
    _condConsumer.notify_one();
}

template<typename T>
bool BlockDeque<T>::Empty() {
    std::lock_guard<std::mutex> locker(_mtx);
    return _deq.empty();
}

template<typename T>
bool BlockDeque<T>::Full() {
    std::lock_guard<std::mutex> locker(_mtx);
    return _deq.size() >= _capacity;
}

// 返回值表示是否pop成功，若成功则将队头元素赋给item
template<typename T>
bool BlockDeque<T>::Pop(T &item) {
    std::unique_lock<std::mutex> locker(_mtx);
    while (_deq.empty()) {
        // 库存为空，所以消费者需要等待嘛
        _condConsumer.wait(locker);
        if (_isClose) return false;
    }
    item = _deq.front();
    _deq.pop_front();
    _condProducer.notify_one(); // 有新的空位了，通知生产者可以继续生产了
    return true;
}

template<typename T>
bool BlockDeque<T>::Pop(T &item, int timeout) {
    std::unique_lock<std::mutex> locker(_mtx);
    whlie (_deq.empty()) {
        if (_condConsumer.wait_for(locker, std::chrono::seconds(timeout)) == std::cv_status::timeout) return false;
        if (_isClose) return false;
    }
    item = _deq.front();
    _deq.pop_front();
    _condProducer.notify_one();
    return true;
}

template<typename T>
BlockDeque<T>::~BlockDeque() {
    Close();
}

#endif