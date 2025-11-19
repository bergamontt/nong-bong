#include "CurrencyDao.h"
#include <soci/soci.h>
#include <soci/sqlite3/soci-sqlite3.h>
#include "CurrencySql.h"

using namespace std;

void CurrencyDao::doCreate(const Currency& currency) const
{
    soci::session sql(_pool);
    sql << currency_sql::create,
        soci::use(currency);
}

std::optional<Currency> CurrencyDao::doGetCurrencyByCode(const string& code) const
{
    soci::session sql(_pool);
    Currency res;
    sql << currency_sql::getByCode,
        soci::use(code, "code"), soci::into(res);
    return sql.got_data() ? std::optional{res} : std::nullopt;
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