#pragma once
#include <soci/soci.h>
#include "ExchangeRate.h"
#include "IExchangeRateDao.h"

class ExchangeRateDao
{
private:
    soci::connection_pool& _pool;

    ExchangeRate doGetById(int id) const;
    std::vector<ExchangeRate> doGetAll() const;
public:
    ExchangeRateDao(soci::connection_pool& pool) : _pool(pool) {}
    ExchangeRateDao(const ExchangeRateDao&) = delete;
    ExchangeRateDao& operator=(const ExchangeRateDao&) = delete;
};