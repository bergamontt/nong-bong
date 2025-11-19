#pragma once
#include <vector>
#include "ExchangeRate.h"

class IExchangeRateDao
{
public:
    void create(const ExchangeRate& rate) const;
    std::optional<ExchangeRate> getById(int id) const;
    std::vector<ExchangeRate> getAll() const;
protected:
    ~IExchangeRateDao() = default;
private:
    virtual void doCreate(const ExchangeRate& rate) const = 0;
    virtual std::optional<ExchangeRate> doGetById(int id) const = 0;
    virtual std::vector<ExchangeRate> doGetAll() const = 0;
};

inline void IExchangeRateDao::create(const ExchangeRate& rate) const
{
    doCreate(rate);
}

inline std::optional<ExchangeRate> IExchangeRateDao::getById(const int id) const
{
    return doGetById(id);
}

inline std::vector<ExchangeRate> IExchangeRateDao::getAll() const
{
    return doGetAll();
}