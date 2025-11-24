#include "ExchangeRateDao.h"
#include "ExchangeRateSql.h"

using namespace std;

void ExchangeRateDao::doCreate(const ExchangeRate& rate) const
{
    soci::session sql(_pool);
    sql << exchange_rate_sql::create,
        soci::use(rate);
}

std::optional<ExchangeRate> ExchangeRateDao::doGetById(int id) const
{
    soci::session sql(_pool);
    ExchangeRate res;
    sql << exchange_rate_sql::getById,
        soci::use(id, "id"), soci::into(res);
    return sql.got_data() ? std::optional{res} : std::nullopt;
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

std::optional<ExchangeRate> ExchangeRateDao::doGetByCurrencies(const std::string& from, const std::string& to) const
{
    soci::session sql(_pool);
    ExchangeRate res;
    sql << exchange_rate_sql::getByCurrencies,
        soci::use(from,"from"),
        soci::use(to,"to"),
        soci::into(res);
    return sql.got_data() ? std::optional{res} : std::nullopt;
}