#pragma once
#include <soci/soci.h>
#include "ExchangeRate.h"
#include "IExchangeRateDao.h"

class ExchangeRateDao final : public IExchangeRateDao
{
public:
    explicit ExchangeRateDao(soci::connection_pool& pool) : _pool(pool) {}
    ExchangeRateDao(const ExchangeRateDao&) = delete;
    ExchangeRateDao& operator=(const ExchangeRateDao&) = delete;
private:
    soci::connection_pool& _pool;

    ExchangeRate doGetById(int id) const override;
    std::vector<ExchangeRate> doGetAll() const override;
};