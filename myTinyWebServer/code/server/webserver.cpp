#include "webserver.h"

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
        // 添加一个定时器，回调函数使用bind绑定为关闭当前连接
        // 注意bind用法，绑定成员函数时，首参数为成员函数地址，第二个参数为调用对象的地址
        _timer->Add(fd, _timeoutMS, std::bind(&WebServer::_CloseConn, this, &_users[fd]));
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

// 调整定时至_timeoutMS后
void WebServer::_ExtentTime(HttpConn *client) {
    assert(client);
    if (_timeoutMS > 0) {
        _timer->Adjust(client->GetFd(), _timeoutMS);
    }
}

void WebServer::_OnProcess(HttpConn *client) {
    if (client->process()) {
        _epoller->ModFd(client->GetFd(), _connEvent | EPOLLOUT);
    }
    else {
        _epoller->ModFd(client->GetFd(), _connEvent | EPOLLIN);
    }
}

void WebServer::_OnRead(HttpConn *client) {
    assert(client);
    int ret = -1;
    int readErrno = 0;
    ret = client->read(&readErrno);
    if (ret <= 0 && readErrno != EAGAIN) {
        _CloseConn(client);
        return;
    }
    _OnProcess(client);
}

void WebServer::_OnWrite(HttpConn *client) {
    assert(client);
    int ret = -1;
    int writeErrno = 0;
    ret = client->write(&writeErrno);
    if (client->ToWriteBytes() == 0) {
        // 传输完成
        if (client->IsKeepAlive()) {
            _OnProcess(client);
            return;
        }
    }
    else if (ret < 0) {
        if (writeErrno == EAGAIN) {
            // 继续传输
            _epoller->ModFd(client->GetFd(), _connEvent | EPOLLOUT);
            return;
        }
    }
    _CloseConn(client);
}

// ???后边再来看
void WebServer::_DealRead(HttpConn *client) {
    assert(client);
    _ExtentTime(client);
    _threadPool->AddTask(std::bind(&WebServer::_OnRead, this, client));
}

void WebServer::_DealWrite(HttpConn *client) {
    assert(client);
    _ExtentTime(client);
    _threadPool->AddTask(std::bind(&WebServer::_OnWrite, this, client));
}

int WebServer::SetFdNonblock(int fd) {
    assert(fd > 0);
    return fcntl(fd, F_SETFL, fcntl(fd, F_GETFD, 0) | O_NONBLOCK);
}

// 创建listenfd
bool WebServer::_InitSocket() {
    int ret;
    struct sockaddr_in addr;
    if (_port > 65535 || _port < 1024) {
        // LOG_ERROR("Port:%d error!", _port);
        return false;
    }
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);   // 通配地址
    addr.sin_port = htons(_port);
    struct linger optLinger = {0};
    if (_openLinger) {
        // 优雅关闭
        optLinger.l_onoff = 1;
        optLinger.l_linger = 1; // 设置等待时长
    }

    // 建立监听描述符
    _listenFd = socket(AF_INET, SOCK_STREAM, 0);
    if (_listenFd < 0) {
        // LOG_ERROR("Create socket error!", _port);
        return false;
    }

    // 设置连接关闭选项
    ret = setsockopt(_listenFd, SOL_SOCKET, SO_LINGER, &optLinger, sizeof(optLinger));
    if (ret < 0) {
        close(_listenFd);
        // LOG_ERROR("Init linger error!", _port);
        return false;
    }
    // 设置端口复用选项
    int optval = 1;
    ret = setsockopt(_listenFd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval, sizeof(int));
    if (ret == -1) {
        // LOG_ERROR("set socket setsockopt error!");
        close(_listenFd);
        return false;
    }

    ret = bind(_listenFd, (struct sockaddr *)&addr, sizeof(addr));
    if (ret < 0) {
        // LOG_ERROR("Bind port:%d error!", _port);
        close(_listenFd);
        return false;
    }
    // 为啥设置为6呢？一般好像都设为1024啊
    ret = listen(_listenFd, 6);
    if (ret < 0) {
        // LOG_ERROR("Listen port:%d error!", _port);
        close(_listenFd);
        return false;
    }
    ret = _epoller->AddFd(_listenFd, _listenEvent | EPOLLIN);
    if (ret == 0) {
        // LOG_ERROR("Add listen error!");
        close(_listenFd);
        return false;
    }
    SetFdNonblock(_listenFd);
    // LOG_INFO("Server port:%d", _port);
    return true;
}

void WebServer::Start() {
    
}

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