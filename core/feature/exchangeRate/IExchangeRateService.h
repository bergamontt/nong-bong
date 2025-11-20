#pragma once
#include <vector>
#include "ExchangeRate.h"

class IExchangeRateService
{
public:
    std::optional<ExchangeRate> getExchangeRateById(int id) const;
    std::vector<ExchangeRate> getAllExchangeRates() const;
    void create(const ExchangeRate& er) const;
    virtual ~IExchangeRateService() = default;
private:
    virtual std::optional<ExchangeRate> doGetExchangeRateById(int id) const = 0;
    virtual std::vector<ExchangeRate> doGetAllExchangeRates() const = 0;
    virtual void doCreate(const ExchangeRate& er) const = 0;
};

inline std::optional<ExchangeRate> IExchangeRateService::getExchangeRateById(const int id) const
{
    return doGetExchangeRateById(id);
}

inline std::vector<ExchangeRate> IExchangeRateService::getAllExchangeRates() const
{
    return doGetAllExchangeRates();
}

inline void IExchangeRateService::create(const ExchangeRate& er) const
{
    return doCreate(er);
}