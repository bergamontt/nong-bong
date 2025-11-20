#pragma once
#include <vector>
#include "Currency.h"
#include "ICurrencyDao.h"
#include "ICurrencyService.h"

class CurrencyService final : public ICurrencyService
{
public:
    explicit CurrencyService(ICurrencyDao& dao);
    CurrencyService(const CurrencyService&) = delete;
    CurrencyService& operator=(const CurrencyService&)& = delete;

private:
    ICurrencyDao& _currencyDao;

    std::optional<Currency> doGetCurrencyByCode(const std::string& code) const override;
    std::vector<Currency> doGetAllCurrencies() const override;
    void doCreate(const Currency &currency) const override;
};