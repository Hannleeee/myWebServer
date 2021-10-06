#ifndef SQLCONNRAII_H
#define SQLCONNRAII_H

#include "sqlconnpool.h"

// 将sql连接的分配封装成类，实现RAII
class SqlConnRAII {
public:
    SqlConnRAII(MYSQL **sql, SqlConnPool *connPool) {
        assert(connPool);
        *sql = connPool->GetConn();
        _sql = *sql;
        _connPool = connPool;
    }

    ~SqlConnRAII() {
        // 检查是否获得了sql连接
        if (_sql) {
            _connPool->FreeConn(_sql);
        }
    }

private:
    MYSQL *_sql;
    SqlConnPool *_connPool;
};

#endif