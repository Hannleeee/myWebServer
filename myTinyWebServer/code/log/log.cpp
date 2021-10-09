#include "log.h"

Log::Log() {
    _lineCount = 0;
    _isAsync = false;
    _writeThread = nullptr;
    _deque = nullptr;
    _toDay = 0;
    _fp = nullptr;
}

int Log::GetLevel() {
    std::lock_guard<std::mutex> locker(_mtx);
    return _level;
}


Log::~Log() {

}