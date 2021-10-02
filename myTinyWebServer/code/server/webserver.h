#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <unordered_map>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "epoller.h"
#include "../log/log.h"
#include "../timer/heaptimer.h"
#include "../pool/sqlconnpool.h"
#include "../pool/threadpool.h"
#include "../pool/sqlconnRAII.h"
#include "../http/httpconn.h"

class WebServer {
public:
    WebServer(
        int port, int trigMode, int timeoutMS, bool optLinger,
        int sqlPort, const char *sqlUser, const char *sqlPwd,
        const char *dbName, int connPoolNum, int threadNum,
        bool openLog, int logLevel, int logQueSize);
    
    ~WebServer();
    void Start();

private:
    bool InitSocket();
    void InitEventMode(int trigMode);
    void AddClient(int fd, sockaddr_in addr);

    void DealListen();
    void DealWrite(HttpConn *client);
    void DealRead(HttpConn *client);

    void SendError(int fd, const char *info);
    void ExtentTime(HttpConn *client);
    void CloseConn(HttpConn *client);

    void OnRead(HttpConn *client);
    void OnWrite(HttpConn *client);
    void OnProcess(HttpConn *client);

    static const int MAX_FD = 65535;

    static int SetFdNonblock(int fd);

    int _port;
    bool _openLinger;
    int _timeoutMS;
    bool _isClose;
    int _listenFd;
    char *_srcDir;

    uint32_t _listenEvent;
    uint32_t _connEvent;

    std::unique_ptr<HeapTimer> _timer;
    std::unique_ptr<ThreadPool> _threadPool;
    std::unique_ptr<Epoller> _epoller;
    std::unordered_map<int, HttpConn> _users;
                
};

#endif