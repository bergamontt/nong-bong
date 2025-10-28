#pragma once
#include <vector>
#include "ExchangeRate.h"
#include "IExchangeRateDao.h"
#include "IExchangeRateService.h"

class ExchangeRateService final : public IExchangeRateService
{
public:
    explicit ExchangeRateService(IExchangeRateDao& dao);
    ExchangeRateService(const ExchangeRateService&) = delete;
    ExchangeRateService& operator=(const ExchangeRateService&)& = delete;

private:
    IExchangeRateDao& _exchangeRateDao;

    std::optional<ExchangeRate> doGetExchangeRateById(int id) const override;
    std::vector<ExchangeRate> doGetAllExchangeRates() const override;
};
