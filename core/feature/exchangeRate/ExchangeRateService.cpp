#include "ExchangeRateService.h"

ExchangeRateService::ExchangeRateService(IExchangeRateDao& dao) :
    _exchangeRateDao(dao)
{}

std::optional<ExchangeRate> ExchangeRateService::doGetExchangeRateById(int id) const {
    return _exchangeRateDao.getById(id);
}

std::vector<ExchangeRate> ExchangeRateService::doGetAllExchangeRates() const {
    return _exchangeRateDao.getAll();
}