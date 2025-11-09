#pragma once
#include <soci/soci.h>
#include "Currency.h"
#include "ICurrencyDao.h"

class CurrencyDao final : public ICurrencyDao
{
public:
    explicit CurrencyDao(soci::connection_pool& pool) : _pool(pool) {}
    CurrencyDao(const CurrencyDao&) = delete;
    CurrencyDao& operator=(const CurrencyDao&)& = delete;
private:
    soci::connection_pool& _pool;
    void doCreate(const Currency &currency) const override;
    Currency doGetCurrencyByCode(const std::string& code) const override;
    std::vector<Currency> doGetAll() const override;
};