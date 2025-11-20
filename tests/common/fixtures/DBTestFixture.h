#pragma once
#include "DataSource.h"
#include <fstream>
#include <sstream>
#include <soci/sqlite3/soci-sqlite3.h>
#include "Schema.h"

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
    sql << schema_sql::createCurrency;
    sql << schema_sql::createExchangeRate;
    sql << schema_sql::createUser;
    sql << schema_sql::createCardDesign;
    sql << schema_sql::createCard;
    sql << schema_sql::createScheduledTransfer;
    sql << schema_sql::createBankTransaction;
}

inline soci::connection_pool& DBTestFixture::pool()
{
    return _ds.pool();
}
