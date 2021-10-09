#ifndef LOG_H
#define LOG_H

#include <mutex>
#include <string>
#include <thread>
#include <sys/time.h>
#include <string.h>
#include <stdarg.h>           // vastart va_end
#include <assert.h>
#include <sys/stat.h>         //mkdir
#include "blockqueue.h"

#include "../buffer/buffer.h"

// 基于单例模式的日志系统
class Log {
public:
    void Init(int level, const char *path = "./log",
                const char *suffix = ".log", int maxQueueCapacity = 1024);
    static Log *Instance();
    static void FlushLogThread();

    void Write(int level, const char *format, ...);
    void Flush();

    int GetLevel();
    void SetLevel(int level);
    bool IsOpen() { return _isOpen; }
private:
    Log();
    virtual ~Log();
    void _AppendLogLevelTitle(int level);
    void _AsyncWrite();

    static const int LOG_PATH_LEN = 256;
    static const int LOG_NAME_LEN = 256;
    static const int MAX_LINES = 50000;

    const char * _path;
    const char *_suffix;

    int _MAX_LINES;
    int _lineCount;
    int _toDay;

    bool _isOpen;

    Buffer _buff;
    int _level;
    bool _isAsync;

    FILE *_fp;
    std::unique_ptr<BlockDeque<std::string>> _deque;
    std::unique_ptr<std::thread> _writeThread;
    std::mutex _mtx;
};

#define LOG_BASE(level, format, ...) \
    do { \
        Log *log = Log::Instance(); \
        if (log->IsOpen() && log->GetLevel() <= level) { \
            log->write(level, format, ##__VA_ARGS__); \
            log->flush(); \
        }\
    } while(0);

#define LOG_DEBUG(format, ...) do{LOG_BASE(0, format, ##__VA_ARGS__)} while(0);
#define LOG_INFO(format, ...) do{LOG_BASE(1, format, ##__VA_ARGS__)} while(0);
#define LOG_WARN(format, ...) do{LOG_BASE(2, format, ##__VA_ARGS__)} while(0);
#define LOG_ERROR(format, ...) do{LOG_BASE(3, format, ##__VA_ARGS__)} while(0);

#endif