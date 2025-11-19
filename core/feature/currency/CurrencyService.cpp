#include "CurrencyService.h"

CurrencyService::CurrencyService(ICurrencyDao& dao)
    : _currencyDao(dao)
{}

std::optional<Currency> CurrencyService::doGetCurrencyByCode(const std::string& code) const
{
    return _currencyDao.getByCode(code);
}

std::vector<Currency> CurrencyService::doGetAllCurrencies() const
{
    return _currencyDao.getAll();
}