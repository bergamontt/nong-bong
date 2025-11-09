#include "ExchangeRateDao.h"
#include "ExchangeRateSql.h"

using namespace std;


void ExchangeRateDao::doCreate(const ExchangeRate& rate) const
{
    soci::session sql(_pool);
    sql << exchange_rate_sql::create,
        soci::use(rate);
}

ExchangeRate ExchangeRateDao::doGetById(int id) const
{
    soci::session sql(_pool);
    ExchangeRate res;
    sql << exchange_rate_sql::getById,
        soci::use(id, "id"), soci::into(res);
    return res;
}

std::vector<ExchangeRate> ExchangeRateDao::doGetAll() const
{
    soci::session sql(_pool);
    vector<ExchangeRate> res;
    soci::rowset<ExchangeRate> rs = (sql.prepare << exchange_rate_sql::getAll);
    for (const ExchangeRate& er : rs)
        res.push_back(er);
    return res;
}