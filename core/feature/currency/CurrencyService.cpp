#include "CurrencyService.h"

CurrencyService::CurrencyService(ICurrencyDao& currDao, IExchangeRateDao& excDao)
    : _currencyDao(currDao)
    , _exchangeRateDao(excDao)
{}

std::optional<Currency> CurrencyService::doGetCurrencyByCode(const std::string& code) const
{
    return _currencyDao.getByCode(code);
}

std::vector<Currency> CurrencyService::doGetAllCurrencies() const
{
    return _currencyDao.getAll();
}

void CurrencyService::doCreate(const Currency &currency) const
{
    return _currencyDao.create(currency);
}

int CurrencyService::doToMinor(const std::string &currencyCode, const int majorAmount) const
{
    const auto curr = _currencyDao.getByCode(currencyCode);
    return majorAmount * curr->minorUnit;
}

int CurrencyService::doConvert(const std::string &fromCurrency, const std::string &toCurrency, int majorAmount) const
{
    const int minorAmount = toMinor(fromCurrency, majorAmount);
    if (fromCurrency == toCurrency)
        return minorAmount;
    auto rates = _exchangeRateDao.getAll();
    for (const auto& r : rates)
        if (r.baseCurrency == fromCurrency && r.targetCurrency == toCurrency)
            return static_cast<int>(minorAmount * r.rate);
    return 0;
}