#pragma once
#include <vector>
#include "ExchangeRate.h"
#include "IExchangeRateDao.h"
#include "IExchangeRateService.h"

class ExchangeRateService final : public IExchangeRateService
{
public:
    explicit ExchangeRateService(IExchangeRateDao& dao);
private:
    IExchangeRateDao& dao;
    [[nodiscard]] std::optional<ExchangeRate> doGetExchangeRateById(int id) const override;
    [[nodiscard]] std::vector<ExchangeRate> doGetAllExchangeRates() const override;
};
