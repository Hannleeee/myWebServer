/* 
参数名\成员变量：驼峰命名法
类名、方法名：首字母大写的驼峰命名 
若为private，则加_前缀
*/
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
    bool _InitSocket();
    void _InitEventMode(int trigMode);
    void _AddClient(int fd, sockaddr_in addr);

    void _DealListen();
    void _DealWrite(HttpConn *client);
    void _DealRead(HttpConn *client);

    void _SendError(int fd, const char *info);
    void _ExtentTime(HttpConn *client);
    void _CloseConn(HttpConn *client);

    void _OnRead(HttpConn *client);
    void _OnWrite(HttpConn *client);
    void _OnProcess(HttpConn *client);

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