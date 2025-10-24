#include "ExchangeRateDao.h"

using namespace std;

ExchangeRate ExchangeRateDao::doGetById(int id) const
{
    soci::session sql(_pool);
    ExchangeRate res;
    sql << "SELECT * FROM exchange_rate WHERE id = :id",
        soci::use(id, "id"), soci::into(res);
    return res;
}

std::vector<ExchangeRate> ExchangeRateDao::doGetAll() const
{
    soci::session sql(_pool);
    vector<ExchangeRate> res;
    soci::rowset<ExchangeRate> rs = (sql.prepare << "SELECT * FROM exchange_rate");
    for (const ExchangeRate& er : rs)
        res.push_back(er);
    return res;
}