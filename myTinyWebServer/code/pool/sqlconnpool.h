#ifndef SQLCONNPOOL_H
#define SQLCONNPOOL_H

#include <mysql/mysql.h>
#include <string>
#include <queue>
#include <mutex>
#include <semaphore.h>
#include <thread>
#include "../log/log.h"

// 使用C接口的单例模式的mySql连接池
class SqlConnPool {
public:
    static SqlConnPool *Instatnce();
    
    MYSQL *GetConn();
    void FreeConn(MYSQL *conn);
    int GetFreeConnCount();

    void Init(const char *host, int port,
              const char *user, const char *pwd,
              const char *dbName, int connSize);
    void ClosePool();

private:
    SqlConnPool();
    ~SqlConnPool();

    int _MAX_CONN;
    int _useCount;
    int _freeCount;

    std::queue<MYSQL *> _connQue;
    std::mutex _mtx;
    sem_t _semId;
};

#endif