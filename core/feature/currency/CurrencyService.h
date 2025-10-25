#pragma once
#include <vector>
#include "Currency.h"
#include "ICurrencyDao.h"
#include "ICurrencyService.h"

class CurrencyService final : public ICurrencyService
{
public:
    explicit CurrencyService(ICurrencyDao& dao);
private:
    ICurrencyDao& dao;
    [[nodiscard]] std::optional<Currency> doGetCurrencyByCode(const std::string& code) const override;
    [[nodiscard]] std::vector<Currency> doGetAllCurrencies() const override;
};