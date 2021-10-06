#include "buffer.h"

// vector<char> str类型的变量，可以只传入一个size，即将str初始化为一个长度为size的空char数组
Buffer::Buffer(int initBufferSize) : _buffer(initBufferSize), _readPos(0), _writePos(0) {}

// 返回指向buffer起始char字符的指针
char *Buffer::_BeginPtr() {
    // 先对迭代器解引用再取地址
    return &*_buffer.begin();
}

const char *Buffer::_BeginPtr() const {
    return &*_buffer.begin();
}

// buffer大小与写入位置之差
size_t Buffer::WriteableBytes() const {
    return _buffer.size() - _writePos;
}

// 即写入位置与已读取位置之差
size_t Buffer::ReadableBytes() const {
    return _writePos - _readPos;
}

// 即读取位置，已读取字符数
size_t Buffer::PrependableBytes() const {
    return _readPos;
}

// 返回指向当前读取位置的指针
const char *Buffer::Peek() const {
    return _BeginPtr() + _readPos;
}

void Buffer::Retrieve(size_t len) {
    assert(len <= ReadableBytes());
    _readPos += len;
}

void Buffer::RetrieveUntil(const char *end) {
    // 确定当前位读取置位于终点之前
    assert(Peek() <= end);
    Retrieve(end - Peek());
}

// 此函数并不用于读取字符，而是用于对buffer的重置
void Buffer::RetrieveAll() {
    bzero(&_buffer[0], _buffer.size());
    _readPos = 0;
    _writePos = 0;
}

// 读取所有剩余字符并转换为string
std::string Buffer::RetrieveAllToStr() {
    std::string str(Peek(), ReadableBytes());
    RetrieveAll();
    return str;
}

const char *Buffer::BeginWriteConst() const {
    return _BeginPtr() + _writePos;
}

char *Buffer::BeginWrite() {
    return _BeginPtr() + _writePos;
}

void Buffer::_MakeSpace(size_t len) {
    // 若进入此函数，则说明尾部空间一定不足以插入，考虑已读取部分的空间
    // 若两部分加一起都不够，直接扩容
    if (WriteableBytes() + PrependableBytes() < len) {
        // 直接将_buffer扩容len，注意resize时，新size和原size的重合部分不会改变
        _buffer.resize(_writePos + len + 1);
    }
    else {  // 否则把已读取前缀符删除
        size_t readable = ReadableBytes();
        // 实际是将未读部分复制到头部，注意并未对原位置清零，只需要改变_readPos和_writePos
        std::copy(_BeginPtr() + _readPos, _BeginPtr() + _writePos, _BeginPtr());
        _readPos = 0;
        _writePos = _readPos + readable;
        // 为啥需要再assert一次？
        assert(readable == ReadableBytes());
    }
}

void Buffer::EnsureWriteable(size_t len) {
    // 先看尾部空间够不够
    if (WriteableBytes() < len) {
        // 若尾部空间不足，则_MakeSpace更新缓冲区，否则do nothing
        _MakeSpace(len);
    }
    assert(WriteableBytes() >= len);
}

// 更新写入位置
void Buffer::HasWritten(size_t len) {
    _writePos += len;
}

void Buffer::Append(const char *str, size_t len) {
    assert(str);
    EnsureWriteable(len);
    std::copy(str, str + len, BeginWrite());
    HasWritten(len);
}

void Buffer::Append(const void *data, size_t len) {
    assert(data);
    Append(static_cast<const char *>(data), len);
}

void Buffer::Append(const std::string &str) {
    Append(str.data(), str.length());
}

void Buffer::Append(const Buffer &buff) {
    Append(buff.Peek(), buff.ReadableBytes());
}

ssize_t Buffer::ReadFd(int fd, int *savedErrno) {
    char buff[65535];
    struct iovec iov[2];    // 用于分散读
    const size_t writeable = WriteableBytes();
    // 分散读，从fd读入，写入buffer！
    // 使用分散读可以保证一次readv读入全部数据，否则需要更复杂的逻辑处理数据超长的情况
    iov[0].iov_base = _BeginPtr() + _writePos;
    iov[0].iov_len = writeable;
    iov[1].iov_base = buff;
    iov[1].iov_len = sizeof(buff);

    // 若读成功，则返回读入的字节数
    const ssize_t len = readv(fd, iov, 2);
    if (len < 0) {
        // 记录errno
        *savedErrno = errno;
    }
    else if (static_cast<size_t>(len) <= writeable) {
        // buffer剩余空间充足
        _writePos += len;
    }
    else {
        // buffer剩余空间不足，超出部分读入buff，然后追加到buffer中（_MakeSpace会自动调整内部空间结构）
        _writePos = _buffer.size();
        Append(buff, len - writeable);
    }
    return len;
}

ssize_t Buffer::WriteFd(int fd, int *savedErrno) {
    size_t readSize = ReadableBytes();
    ssize_t len = write(fd, Peek(), readSize);
    if (len < 0) {
        *savedErrno = errno;
        return len;
    }
    _readPos += len;
    return len;
}