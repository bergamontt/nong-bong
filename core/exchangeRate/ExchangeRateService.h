#pragma once
#include <vector>
#include "ExchangeRate.h"
#include "IExchangeRateDao.h"
#include "IExchangeRateService.h"

class ExchangeRateService : public IExchangeRateService
{
    std::shared_ptr<IExchangeRateDao> dao;

    explicit ExchangeRateService(std::shared_ptr<IExchangeRateDao> dao);

    [[nodiscard]] std::optional<ExchangeRate> doGetExchangeRateById(int id) const override;
    [[nodiscard]] std::vector<ExchangeRate> doGetAllExchangeRates() const override;
};
