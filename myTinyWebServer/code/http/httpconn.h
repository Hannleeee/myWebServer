#ifndef HTTP_CONN_H
#define HTTP_CONN_H

#include <sys/types.h>
#include <sys/uio.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <errno.h>

#include "../log/log.h"
#include "../pool/sqlconnRAII.h"
#include "../buffer/buffer.h"
#include "httprequest.h"
#include "httpresponse.h"

class HttpConn {
public:
    HttpConn();

    ~HttpConn();

    void init(int sockfd, const sockaddr_in &addr);

    ssize_t read(int *saveErrno);

    ssize_t write(int *saveErrno);

    void Close();

    int GetFd() const;
    int GetPort() const;
    const char *GetIP() const;
    sockaddr_in GetAddr() const;

    bool process();

    int ToWriteBytes() {
        return _iov[0].iov_len + _iov[1].iov_len;
    }

    bool IsKeepAlive() const {
        return _request.IsKeepAlive();
    }

    static bool isET;
    static const char *srcDir;
    static std::atomic<int> userCount;

private:
    int _fd;
    struct sockaddr_in _addr;

    bool _isClose;

    int _iovCnt;
    struct iovec _iov[2];

    Buffer _readBuff;
    Buffer _writeBuff;

    HttpRequest _request;
    HttpResponse _response;
};

#endif