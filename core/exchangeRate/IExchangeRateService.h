#pragma once
#include <vector>
#include "ExchangeRate.h"

class IExchangeRateService
{
public:
    [[nodiscard]] std::optional<ExchangeRate> getExchangeRateById(int id) const;
    [[nodiscard]] std::vector<ExchangeRate> getAllExchangeRates() const;
    virtual ~IExchangeRateService() = default;
private:
    [[nodiscard]] virtual std::optional<ExchangeRate> doGetExchangeRateById(int id) const = 0;
    [[nodiscard]] virtual std::vector<ExchangeRate> doGetAllExchangeRates() const = 0;
};

inline std::optional<ExchangeRate> IExchangeRateService::getExchangeRateById(const int id) const { return doGetExchangeRateById(id); }
inline std::vector<ExchangeRate> IExchangeRateService::getAllExchangeRates() const { return doGetAllExchangeRates(); }