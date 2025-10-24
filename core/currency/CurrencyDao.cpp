#include "CurrencyDao.h"
#include <soci/soci.h>
#include <soci/sqlite3/soci-sqlite3.h>
#include <iostream>

using namespace std;

Currency CurrencyDao::doGetCurrencyByCode(const string& code) const
{
    soci::session sql(_pool);
    Currency res;
    sql << "SELECT * FROM currency WHERE code = :code",
        soci::use(code, "code"), soci::into(res);
    if (!sql.got_data())
        throw runtime_error("Currency not found: " + code);
    return res;
}

vector<Currency> CurrencyDao::doGetAll() const
{
    soci::session sql(soci::sqlite3, "database.sqlite");
    vector<Currency> res;
    soci::rowset<Currency> rs = (sql.prepare << "SELECT * FROM currency");
    for (const Currency& c : rs)
        res.push_back(c);
    return res;
}