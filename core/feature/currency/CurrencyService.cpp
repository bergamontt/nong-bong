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
    const auto rate = _exchangeRateDao.getByCurrencies(fromCurrency, toCurrency);
    if (!rate.has_value()) return 0;
    return static_cast<int>(minorAmount * rate->rate);
}