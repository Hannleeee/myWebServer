#include "httpconn.h"

const char* HttpConn::srcDir;
std::atomic<int> HttpConn::userCount;
bool HttpConn::isET;

HttpConn::HttpConn() {
    _fd = -1;
    _addr = { 0 };
    _isClose = true;
}

void HttpConn::init(int fd, const sockaddr_in &addr) {
    assert(fd > 0);
    userCount++;
    _addr = addr;
    _fd = fd;
    _writeBuff.RetrieveAll();
    _readBuff.RetrieveAll();
    _isClose = false;
    LOG_INFO("Client[%d](%s:%d) in, userCount:%d", _fd, GetIP(), GetPort(), (int)userCount);
}

void HttpConn::Close() {

}

int HttpConn::GetFd() const {
    return _fd;
}

struct sockaddr_in HttpConn::GetAddr() const {
    return _addr;
}

const char *HttpConn::GetIP() const {
    return inet_ntoa(_addr.sin_addr);   // 转换为点分十进制IP表示
}

int HttpConn::GetPort() const {
    return _addr.sin_port;
}

ssize_t HttpConn::read(int *saveErrno) {
    ssize_t len = -1;
    do {
        len = _readBuff.ReadFd(_fd, saveErrno);
        if (len <= 0) {
            break;
        }
    } while (isET);
    return len;
}

// 再研究一下，没太看懂
ssize_t HttpConn::write(int *saveErrno) {
    ssize_t len = -1;
    do {
        // 将iov中的buffers的数据写入fd
        len = writev(_fd, _iov, _iovCnt);
        if (len <= 0) {
            *saveErrno = errno;
            break;
        }
        if (_iov[0].iov_len + _iov[1].iov_len == 0) {
            // 传输完成
            break;
        }
        else if (static_cast<size_t>(len) > _iov[0].iov_len) {
            // _iov[0]全部写入，_iov[1]部分写入
            _iov[1].iov_base = (uint8_t *)_iov[1].iov_base + (len - _iov[0].iov_len);
            _iov[1].iov_len -= (len - _iov[0].iov_len);
            if (_iov[0].iov_len) {
                _writeBuff.RetrieveAll();
                _iov[0].iov_len = 0;
            }
        }
        else {
            // 仅写入了_iov[0]
            _iov[0].iov_base = (uint8_t *)_iov[0].iov_base + len;
            _iov[0].iov_len -= len;
            _writeBuff.Retrieve(len);
        }
    } while (isET || ToWriteBytes() > 10240);
    return len;
}

bool HttpConn::process() {
    _request.Init();
    if (_readBuff.ReadableBytes() <= 0) {
        return false;
    }
    else if (_request.parse(_readBuff)) {
        LOG_DEBUG("%s", _request.path().c_str());
        _response.Init();
    } else {
        _response.Init();
    }

    _
}

HttpConn::~HttpConn() {
    Close();
}