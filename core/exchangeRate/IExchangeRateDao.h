#pragma once
#include <vector>
#include "ExchangeRate.h"

class IExchangeRateDao
{
public:
    ExchangeRate getById(int id) const;
    std::vector<ExchangeRate> getAll() const;
    virtual ~IExchangeRateDao() = default;
private:
    virtual ExchangeRate doGetById(int id) const = 0;
    virtual std::vector<ExchangeRate> doGetAll() const = 0;
};

inline ExchangeRate IExchangeRateDao::getById(const int id) const
{
    return doGetById(id);
}

inline std::vector<ExchangeRate> IExchangeRateDao::getAll() const
{
    return doGetAll();
}