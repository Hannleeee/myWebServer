#include "webserver.h"

WebServer::WebServer(
    int port, int trigMode, int timeoutMS, bool optLinger,
    int sqlPort, const char *sqlUser, const char *sqlPwd,
    const char *dbName, int connPoolNum, int threadNum,
    bool openLog, int logLevel, int logQueSize) : _port(port),
    _openLinger(optLinger), _timeoutMS(timeoutMS), _isClose(false),
    _timer(new HeapTimer()), _threadPool(new ThreadPool(threadNum)),
    _epoller(new Epoller()) {
    // 获取当前路径
    _srcDir = getcwd(nullptr, 256);
    assert(_srcDir);
    // 添加路径后缀
    strncat(_srcDir, "/resource/", 16);
    HttpConn::userCount = 0;
    HttpConn::srcDir = _srcDir;
    SqlConnPool::Instatnce()->Init("localhost", sqlPort, sqlUser, sqlPwd, dbName, connPoolNum);

}

WebServer::~WebServer() {
    close(_listenFd);
    _isClose = true;
    free(_srcDir);
    SqlConnPool::Instatnce()->ClosePool();
}

void WebServer::_InitEventMode(int trigMode) {
    _listenEvent = EPOLLRDHUP;
    _connEvent = EPOLLONESHOT | EPOLLRDHUP;
    switch (trigMode) {
        case 0:
            break;
        case 1:
            _connEvent |= EPOLLET;
            break;
        case 2:
            _listenEvent |= EPOLLET;
            break;
        case 3:
            _listenEvent |= EPOLLET;
            _connEvent |= EPOLLET;
            break;
        default:
            _listenEvent |= EPOLLET;
            _connEvent |= EPOLLET;
            break;
    }
    HttpConn::isET = (_connEvent & EPOLLET);
}

void WebServer::_SendError(int fd, const char *info) {
    assert(fd > 0);
    int ret = send(fd, info, strlen(info), 0);
    if (ret < 0) {
        // LOG_WARN("send error to client[%d] error!", fd);
    }
    close(fd);
}

void WebServer::_CloseConn(HttpConn *client) {
    assert(client);
    // LOG_INFO("Client[%d] quit!", client->GetFd());
    _epoller->DelFd(client->GetFd());
    client->Close();
}

void WebServer::_AddClient(int fd, sockaddr_in addr) {
    assert(fd > 0);
    _users[fd].init(fd, addr);
    if (_timeoutMS > 0) {
        // 估计要看完Timer才知道啥意思
        _timer->add(fd, _timeoutMS, std::bind(&WebServer::_CloseConn, this, &_users[fd]));
    }
    _epoller->AddFd(fd, EPOLLIN | _connEvent);
    SetFdNonblock(fd);
    // LOG_INFO("Client[%d] in!", _user[fd].GetFd());
}

void WebServer::_DealListen() {
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    do {
        int fd = accept(_listenFd, (struct sockaddr *)&addr, &len);
        if (fd <= 0) return ;
        else if (HttpConn::userCount >= MAX_FD) {
            _SendError(fd, "Server busy!");
            // LOG_WARN("Client is full!");
            return;
        }
        _AddClient(fd, addr);
    } while (_listenEvent & EPOLLET);
}
