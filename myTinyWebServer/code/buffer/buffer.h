#ifndef BUFFER_H
#define BUFFER_H

#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/uio.h>
#include <vector>
#include <atomic>
#include <assert.h>

class Buffer {
public:
    Buffer(int bufferSize = 1024);
    ~Buffer() = default;

    size_t WriteableBytes() const;
    size_t ReadableBytes() const;
    size_t PrependableBytes() const;

    const char *Peek() const;
    void EnsureWriteable(size_t len);
    void HasWritten(size_t len);

    void Retrieve(size_t len);
    void RetrieveUntil(const char *end);
    void RetrieveAll();

    std::string RetrieveAllToStr();

    const char *BeginWriteConst() const;
    char *BeginWrite();

    void Append(const std::string &str);
    void Append(const char *str, size_t len);
    void Append(const void *data, size_t len);
    void Append(const Buffer &buff);

    ssize_t ReadFd(int fd, int *Errno);
    ssize_t WriteFd(int fd, int *Errno);

private:
    char *_BeginPtr();
    const char *_BeginPtr() const;
    void _MakeSpace(size_t len);

    std::vector<char> _buffer;  // 使用vector封装char作为buffer
    std::atomic<std::size_t> _readPos;  // 当前buffer读取位置
    std::atomic<std::size_t> _writePos; // 当前buffer写入位置
};

#endif