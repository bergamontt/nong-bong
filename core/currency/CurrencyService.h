#pragma once
#include <vector>
#include "Currency.h"
#include "ICurrencyDao.h"
#include "ICurrencyService.h"

class CurrencyService : public ICurrencyService
{
    std::shared_ptr<ICurrencyDao> dao;

    explicit CurrencyService(std::shared_ptr<ICurrencyDao> dao);

    std::optional<Currency> doGetCurrencyByCode(const std::string& code) const override;
    std::vector<Currency> doGetAllCurrencies() const override;
};