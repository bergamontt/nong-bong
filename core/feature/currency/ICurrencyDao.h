#pragma once
#include <vector>
#include "Currency.h"

class ICurrencyDao
{
public:
    void create(const Currency& currency) const;
    Currency getByCode(const std::string& code) const;
    std::vector<Currency> getAll() const;
protected:
    ~ICurrencyDao() = default;
private:
    virtual void doCreate(const Currency& currency) const = 0;
    virtual Currency doGetCurrencyByCode(const std::string& code) const = 0;
    virtual std::vector<Currency> doGetAll() const = 0;
};

inline void ICurrencyDao::create(const Currency& currency) const
{
    doCreate(currency);
}

inline Currency ICurrencyDao::getByCode(const std::string& code) const
{
    return doGetCurrencyByCode(code);
}

inline std::vector<Currency> ICurrencyDao::getAll() const
{
    return doGetAll();
}