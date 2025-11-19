#pragma once
#include <vector>
#include "Currency.h"

class ICurrencyService
{
public:
    std::optional<Currency> getCurrencyByCode(const std::string& code) const;
    std::vector<Currency> getAllCurrencies() const;
    virtual ~ICurrencyService() = default;
private:
    virtual std::optional<Currency> doGetCurrencyByCode(const std::string& code) const = 0;
    virtual std::vector<Currency> doGetAllCurrencies() const = 0;
};

inline std::optional<Currency> ICurrencyService::getCurrencyByCode(const std::string& code) const
{
    return doGetCurrencyByCode(code);
}

inline std::vector<Currency> ICurrencyService::getAllCurrencies() const
{
    return doGetAllCurrencies();
}