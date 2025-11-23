#pragma once
#include <vector>
#include "Currency.h"
#include "ICurrencyDao.h"
#include "IExchangeRateDao.h"
#include "ICurrencyService.h"

class CurrencyService final : public ICurrencyService
{
public:
    explicit CurrencyService(ICurrencyDao& currDao, IExchangeRateDao& excDao);
    CurrencyService(const CurrencyService&) = delete;
    CurrencyService& operator=(const CurrencyService&)& = delete;

private:
    ICurrencyDao& _currencyDao;
    IExchangeRateDao& _exchangeRateDao;

    std::optional<Currency> doGetCurrencyByCode(const std::string& code) const override;
    std::vector<Currency> doGetAllCurrencies() const override;
    void doCreate(const Currency &currency) const override;
    int doToMinor(const std::string& currencyCode, int majorAmount) const override;
    int doConvert(const std::string& fromCurrency, const std::string& toCurrency, int majorAmount) const override;
};