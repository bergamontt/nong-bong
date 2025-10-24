#pragma once
#include <soci/soci.h>
#include "Currency.h"
#include "ICurrencyDao.h"

class CurrencyDao : public ICurrencyDao
{
private:
    soci::connection_pool& _pool;

    Currency doGetCurrencyByCode(const std::string& code) const;
    std::vector<Currency> doGetAll() const;
public:
    CurrencyDao(soci::connection_pool& pool) : _pool(pool) {}
    CurrencyDao(const CurrencyDao&) = delete;
    CurrencyDao& operator=(const CurrencyDao&)& = delete;
};