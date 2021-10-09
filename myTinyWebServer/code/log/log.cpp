#include "log.h"

Log *Log::Instance() {
    static Log inst;
    return &inst;
}

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

void Log::SetLevel(int level) {
    std::lock_guard<std::mutex> locker(_mtx);
    _level = level;
}

// 刷新缓冲区
void Log::Flush() {
    if (_isAsync) {
        _deque->Flush();
    }
    fflush(_fp);
}

void Log::_AsyncWrite() {
    std::string str = "";
    // 从队列中获取待写入内容写入日志
    while (_deque->Pop(str)) {
        std::lock_guard<std::mutex> locker(_mtx);
        fputs(str.c_str(), _fp);
    }
}

// 强制刷新缓冲区写入日志
void Log::FlushLogThread() {
    Log::Instance()->_AsyncWrite();
}

void Log::Init(int level = 1, const char *path, const char *suffix, int maxQueueSize) {
    _isOpen = true;
    _level = level;
    if (maxQueueSize > 0) {
        // 创建阻塞队列实现异步写
        _isAsync = true;
        if (!_deque) {
            std::unique_ptr<BlockDeque<std::string>> newDeque(new BlockDeque<std::string>);
            _deque = std::move(newDeque);
            // 创建用于异步写的新线程
            std::unique_ptr<std::thread> newThread(new std::thread(FlushLogThread));
            _writeThread = std::move(newThread);
        }
    } else {
        _isAsync = false;
    }

    _lineCount = 0;

    time_t timer = time(nullptr);
    struct tm *sysTime = localtime(&timer);
    struct tm t = *sysTime;
    _path = path;
    _suffix = suffix;
    char fileName[LOG_NAME_LEN] = {0};
    // tm中，tm_year存储自1900过了多少年，其余成员中，除了tm_mday（当月第几天）是从1开始，全部都是从0开始，所以月份需要+1！
    snprintf(fileName, LOG_NAME_LEN - 1, "%s/%04d_%02d_%02d%s", _path, t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, _suffix);
    _toDay = t.tm_mday;

    {
        std::lock_guard<std::mutex> locker(_mtx);
        _buff.RetrieveAll();
        // 如果_fp已有指向，则清空并关闭，进行重置
        if (_fp) {
            Flush();
            fclose(_fp);
        }

        // append模式，若文件已存在则追加，否则创建。若失败则返回nullptr，说明没有对应路径
        _fp = fopen(fileName, "a");
        // 若打开失败，则创建对应路径
        if (_fp == nullptr) {
            // C++中八进制以0开头！权限设置为rwx rwx rwx(owner,group, other users)
            mkdir(_path, 0777);
            _fp = fopen(fileName, "a");
        }
        assert(_fp != nullptr);
    }
}

void Log::_AppendLogLevelTitle(int level) {
    switch(level) {
        case 0:
            _buff.Append("[debug]: ", 9);
            break;
        case 1:
            _buff.Append("[info]: ", 9);
            break;
        case 2:
            _buff.Append("[warn]: ", 9);
            break;
        case 3:
            _buff.Append("[error]: ", 9);
            break;
        default:
            _buff.Append("[info]: ", 9);
            break;
    }
}

void Log::Write(int level, const char *format, ...) {
    struct timeval now = {0, 0};
    gettimeofday(&now, nullptr);
    time_t tSec = now.tv_sec;
    struct tm *sysTime = localtime(&tSec);
    struct tm t = *sysTime;     // 获取当前时间
    va_list vaList;     // 可变参数列表，就是printf用的那种

    // 需要更新日期，或者是当日日志行数已超过最大行数，需要拆分为新的文件
    if (_toDay != t.tm_mday || (_lineCount && (_lineCount % MAX_LINES == 0))) {
        std::unique_lock<std::mutex> locker(_mtx, std::defer_lock);

        char newFile[LOG_NAME_LEN];
        char tail[36] = {0};    // 日期后缀
        snprintf(tail, 36, "%04d_%02d_%02d", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday);

        if (_toDay != t.tm_mday) {
            snprintf(newFile, LOG_NAME_LEN - 72, "%s/%s%s", _path, tail, _suffix);
            _toDay = t.tm_mday;
            _lineCount = 0;     // 刚更新日期，那当然是当日第一个文件了，_linecount重置为0
        }
        else {
            snprintf(newFile, LOG_NAME_LEN - 72, "%s/%s-%d%s", _path, tail, (_lineCount / MAX_LINES), _suffix);
        }

        // 这里是在干啥？？？
        locker.lock();
        Flush();
        fclose(_fp);
        _fp = fopen(newFile, "a");
        assert(_fp != nullptr);
    }

    {
        std::unique_lock<std::mutex> lcoker(_mtx);
        ++_lineCount;
        int n = snprintf(_buff.BeginWrite(), 128, "%d-%02d-%02d %02d:%02d:%02d.%06ld ", 
                    t.tm_year + 1900, t.tm_mon + 1, t.tm_mday,
                    t.tm_hour, t.tm_min, t.tm_sec, now.tv_usec);
        _buff.HasWritten(n);
        _AppendLogLevelTitle(level);    // 不应该放在前边吗？？？

        va_start(vaList, format);       // 可变参数初始化
        // 涉及可变参数的专用snprintf
        int m = vsnprintf(_buff.BeginWrite(), _buff.WriteableBytes(), format, vaList);
        va_end(vaList);                 // 结束可变参数的获取

        _buff.HasWritten(m);
        _buff.Append("\n\0", 2);        // ？用处？

        // 阻塞队列未满
        if (_isAsync && _deque && !_deque->Full()) {
            _deque->PushBack(_buff.RetrieveAllToStr());
        } else {
            fputs(_buff.Peek(), _fp);
        }
        _buff.RetrieveAll();
    }
}

Log::~Log() {
    // 关闭写线程
    if (_writeThread && _writeThread->joinable()) {
        while (!_deque->Empty()) _deque->Flush();
        _deque->Close();
        _writeThread->join();
    }
    // 关闭文件
    if (_fp) {
        std::lock_guard<std::mutex> locker(_mtx);
        Flush();
        fclose(_fp);
    }
}