#include "heaptimer.h"

void HeapTimer::_SwapNode(size_t i, size_t j) {
    assert(i >= 0 && i < _heap.size());
    assert(j >= 0 && j < _heap.size());
    std::swap(_heap[i], _heap[j]);
    _ref[_heap[i].id] = i;
    _ref[_heap[j].id] = j;
}

void HeapTimer::_SiftUp(size_t i) {
    assert(i >= 0 && i < _heap.size());
    // 即为父节点索引
    size_t j = (i - 1) / 2;
    while (j >= 0) {
        // 若父节点值小于当前节点值，则停止上滤
        if (_heap[j] < _heap[i]) break;
        // 否则交换二者，并更新当前节点为父节点，以及更新父节点索引
        _SwapNode(i, j);
        i = j;
        j = (i - 1) / 2;
    }
}

// 返回值表示下滤是否成功
bool HeapTimer::_SiftDown(size_t index, size_t n) {
    assert(index >= 0 && index < _heap.size());
    assert(n >= 0 && n <= _heap.size());
    size_t i = index;
    size_t j = i * 2 + 1;   // 即为当前节点左孩子
    // 注意n是开区间边界，即不取n
    while (j < n) {
        // 若右孩子存在，且小于左孩子，则只考虑右孩子，也即，在此j代表的是子孩子中的较小者
        if (j + 1 < n && _heap[j+1] < _heap[j]) ++j;
        // 若当前节点比孩子们更小，则停止下滤
        if (_heap[i] < _heap[j]) break;
        _SwapNode(i, j);
        i = j;
        j = 2 * i + 1;
    }
    return i > index;
}

void HeapTimer::Add(int id, int timeout, const TimeoutCallBack &cb) {
    assert(id > 0);
    size_t i;
    if (_ref.count(id) == 0) {
        // 为新任务，则插入堆尾，调整堆
        i = _heap.size();
        _ref[id] = i;
        _heap.push_back({id, Clock::now() + MS(timeout), cb});
        _SiftUp(i);
    }
    else {
        // 任务已存在，对其进行状态更新
        i = _ref[id];
        _heap[i].expires = Clock::now() + MS(timeout);
        _heap[i].cb = cb;
        // 若下滤返回false，说明需要进行的是上滤
        if (!_SiftDown(i, _heap.size())) _SiftUp(i);
    }
}

// 通过将对应节点与末尾节点交换，并进行一次下滤实现删除指定节点
void HeapTimer::_Del(size_t index) {
    assert(!_heap.empty() && index >= 0 && index < _heap.size());
    size_t i = index;
    size_t n = _heap.size() - 1;
    assert(i <= n);
    if (i < n) {
        _SwapNode(i, n);
        if (!_SiftDown(i, n)) _SiftUp(i);
    }
    // 删除记录
    _ref.erase(_heap.back().id);
    _heap.pop_back();
}

void HeapTimer::DoWork(int id) {
    if (!_heap.empty() || _ref.count(id) == 0) {
        return;
    }
    size_t i = _ref[id];
    _heap[i].cb();
    _Del(i);
}

// 修改任务的定时
void HeapTimer::Adjust(int id, int timeout) {
    assert(!_heap.empty() && _ref.count(id));
    size_t i = _ref[id];
    _heap[i].expires = Clock::now() + MS(timeout);
    if (!_SiftDown(i, _heap.size())) _SiftUp(i);
}

// 弹出堆顶元素
void HeapTimer::Pop() {
    assert(_heap.empty());
    _Del(0);
}

void HeapTimer::Tick() {
    if (_heap.empty()) return;
    while (!_heap.empty()) {
        // 堆顶的定时器还没到时间，无需操作直接退出
        if (std::chrono::duration_cast<MS>(_heap.front().expires - Clock::now()).count() > 0) break;
        _heap.front().cb();
        Pop();
    }
}

// 清空定时器
void HeapTimer::Clear() {
    _ref.clear();
    _heap.clear();
}

// 返回值类型应该为什么呢？？？
int HeapTimer::GetNextTick() {
    Tick();
    long long res = -1;
    if (!_heap.empty()) {
        res = std::chrono::duration_cast<MS>(_heap.front().expires - Clock::now()).count();
        if (res < 0) res = 0;
    }
    return res;
}