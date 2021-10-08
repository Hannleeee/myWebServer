#ifndef HEAP_TIMER_H
#define HEAP_TIMER_H

#include <queue>
#include <unordered_map>
#include <time.h>
#include <algorithm>
#include <arpa/inet.h>
#include <functional>
#include <assert.h>
#include <chrono>

#include "../log/log.h"

typedef std::function<void()> TimeoutCallBack;
typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::milliseconds MS;
typedef Clock::time_point TimeStamp;

struct TimerNode {
    int id;
    TimeStamp expires;
    TimeoutCallBack cb;
    // 重载比较运算符，使其适用于优先队列
    bool operator<(const TimerNode &t) {
        return expires < t.expires;
    }
};

class HeapTimer {
public:
    HeapTimer() {
        _heap.reserve(64);
    }

    void Adjust(int id, int newExpires);

    void Add(int id, int timeout, const TimeoutCallBack &cb);

    void DoWork(int id);

    void Clear();
    void Tick();
    void Pop();
    int GetNextTick();

    ~HeapTimer() {
        Clear();
    }

private:
    void _Del(size_t i);
    void _SiftUp(size_t i);
    bool _SiftDown(size_t index, size_t n);
    void _SwapNode(size_t i, size_t j);

    std::vector<TimerNode> _heap;           // 即堆数组
    std::unordered_map<int, size_t> _ref;   // 用于由任务id获取在堆中位置，实现删除和调整的功能
};

#endif