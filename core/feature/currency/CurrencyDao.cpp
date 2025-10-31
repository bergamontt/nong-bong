#include "CurrencyDao.h"
#include <soci/soci.h>
#include <soci/sqlite3/soci-sqlite3.h>
#include "CurrencySql.h"

using namespace std;

Currency CurrencyDao::doGetCurrencyByCode(const string& code) const
{
    soci::session sql(_pool);
    Currency res;
    sql << currency_sql::getByCode,
        soci::use(code, "code"), soci::into(res);
    return res;
}

vector<Currency> CurrencyDao::doGetAll() const
{
    soci::session sql(_pool);
    vector<Currency> res;
    soci::rowset<Currency> rs =
        (sql.prepare << currency_sql::getAll);
    for (const Currency& c : rs)
        res.push_back(c);
    return res;
}