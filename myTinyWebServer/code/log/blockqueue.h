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

    void clear();
};

#endif