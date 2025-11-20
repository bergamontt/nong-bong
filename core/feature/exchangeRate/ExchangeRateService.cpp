#include "ExchangeRateService.h"

ExchangeRateService::ExchangeRateService(IExchangeRateDao& dao)
    : _exchangeRateDao(dao)
{}

std::optional<ExchangeRate> ExchangeRateService::doGetExchangeRateById(const int id) const
{
    return _exchangeRateDao.getById(id);
}

std::vector<ExchangeRate> ExchangeRateService::doGetAllExchangeRates() const
{
    return _exchangeRateDao.getAll();
}

void ExchangeRateService::doCreate(const ExchangeRate& er) const
{
    return _exchangeRateDao.create(er);
}