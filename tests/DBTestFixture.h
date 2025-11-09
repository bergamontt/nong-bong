#pragma once
#include "DataSource.h"
#include <fstream>
#include <sstream>
#include <soci/sqlite3/soci-sqlite3.h>

class DBTestFixture
{
public:
    DBTestFixture();
    soci::connection_pool& pool();
private:
    DataSource _ds;
    static void createSchema(soci::session& sql);
};

inline DBTestFixture::DBTestFixture()
    :  _ds(":memory:", soci::sqlite3, 1)
{
    soci::session& sql = _ds.pool().at(0);
    createSchema(sql);
}

inline void DBTestFixture::createSchema(soci::session& sql)
{
    std::string sqlText = R"(
        CREATE TABLE user (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            first_name TEXT NOT NULL,
            last_name TEXT NOT NULL,
            email TEXT UNIQUE,
            phone TEXT NOT NULL UNIQUE,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            password_hash TEXT NOT NULL,
            status TEXT NOT NULL DEFAULT 'active' CHECK (status IN ('active','blocked','deleted')),
            failed_login_count INTEGER NOT NULL DEFAULT 0,
            blocked_until DATETIME
        );
    )";

    sql << sqlText;
}

inline soci::connection_pool& DBTestFixture::pool()
{
    return _ds.pool();
}
