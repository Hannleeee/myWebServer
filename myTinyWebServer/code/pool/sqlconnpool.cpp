#include "sqlconnpool.h"

SqlConnPool::SqlConnPool() {
    _useCount = 0;
    _freeCount = 0;
}

// 获取唯一实例
SqlConnPool *SqlConnPool::Instatnce() {
    // 声明为static，保证唯一性
    static SqlConnPool connPool;
    return &connPool;
}

void SqlConnPool::Init(const char *host, int port,
            const char *user, const char *pwd, const char *dbName,
            int connSize = 10) {
    assert(connSize > 0);
    for (int i = 0; i < connSize; ++i) {
        MYSQL *sql = nullptr;
        sql = mysql_init(sql);
        if (!sql) {
            // LOG_ERROR("Mysql init Error");
            assert(sql);
        }
        sql = mysql_real_connect(sql, host, user, pwd, dbName, port, nullptr, 0);
        if (!sql) {
            // LOG_ERROR("Mysql Connect Error");
        }
        _connQue.push(sql);
    }
    _MAX_CONN = connSize;
    sem_init(&_semId, 0, _MAX_CONN);
}

MYSQL *SqlConnPool::GetConn() {
    MYSQL *sql = nullptr;
    // 若无空闲sql连接，则返回空指针
    if (_connQue.empty()) {
        // LOG_WARN("SqlConnPool busy!");
        return nullptr;
    }
    sem_wait(&_semId);
    {
        std::lock_guard<std::mutex> locker(_mtx);
        sql = _connQue.front();
        _connQue.pop();
    }
    return sql;
}

void SqlConnPool::FreeConn(MYSQL *sql) {
    assert(sql);
    std::lock_guard<std::mutex> locker(_mtx);
    _connQue.push(sql);
    sem_post(&_semId);
}

void SqlConnPool::ClosePool() {
    std::lock_guard<std::mutex> locker(_mtx);
    while (!_connQue.empty()) {
        auto item = _connQue.front();
        _connQue.pop();
        mysql_close(item);
    }
    // 终止使用mysql
    mysql_library_end();
}

int SqlConnPool::GetFreeConnCount() {
    std::lock_guard<std::mutex> locker(_mtx);
    return _connQue.size();
}

SqlConnPool::~SqlConnPool() {
    ClosePool();
}