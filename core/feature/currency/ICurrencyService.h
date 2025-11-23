#pragma once
#include <vector>
#include "Currency.h"

class ICurrencyService
{
public:
    std::optional<Currency> getCurrencyByCode(const std::string& code) const;
    std::vector<Currency> getAllCurrencies() const;
    void create(const Currency& currency) const;
    int toMinor(const std::string& currencyCode, int majorAmount) const;
    int convert(const std::string& fromCurrency, const std::string& toCurrency, int majorAmount) const;
    virtual ~ICurrencyService() = default;
private:
    virtual std::optional<Currency> doGetCurrencyByCode(const std::string& code) const = 0;
    virtual std::vector<Currency> doGetAllCurrencies() const = 0;
    virtual void doCreate(const Currency& currency) const = 0;
    virtual int doToMinor(const std::string& currencyCode, int majorAmount) const = 0;
    virtual int doConvert(const std::string& fromCurrency, const std::string& toCurrency, int majorAmount) const = 0;
};

inline std::optional<Currency> ICurrencyService::getCurrencyByCode(const std::string& code) const
{
    return doGetCurrencyByCode(code);
}

inline std::vector<Currency> ICurrencyService::getAllCurrencies() const
{
    return doGetAllCurrencies();
}

inline void ICurrencyService::create(const Currency& currency) const
{
    doCreate(currency);
}

inline int ICurrencyService::toMinor(const std::string& currencyCode, int majorAmount) const
{
    return doToMinor(currencyCode, majorAmount);
}

inline int ICurrencyService::convert(const std::string &fromCurrency, const std::string &toCurrency, int majorAmount) const
{
    return doConvert(fromCurrency, toCurrency, majorAmount);
}
