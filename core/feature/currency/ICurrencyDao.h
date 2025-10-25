#pragma once
#include <vector>
#include "Currency.h"

class ICurrencyDao
{
public:
    Currency getByCode(const std::string& code) const;
    std::vector<Currency> getAll() const;
    virtual ~ICurrencyDao() = default;
private:
    virtual Currency doGetCurrencyByCode(const std::string& code) const = 0;
    virtual std::vector<Currency> doGetAll() const = 0;
};

inline Currency ICurrencyDao::getByCode(const std::string& code) const
{
    return doGetCurrencyByCode(code);
}

inline std::vector<Currency> ICurrencyDao::getAll() const
{
    return doGetAll();
}