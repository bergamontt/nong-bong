#pragma once
#include <vector>
#include "Currency.h"

class ICurrencyService
{
public:
    [[nodiscard]] std::optional<Currency> getCurrencyByCode(const std::string& code) const;
    [[nodiscard]] std::vector<Currency> getAllCurrencies() const;
    virtual ~ICurrencyService() = default;
private:
    [[nodiscard]] virtual std::optional<Currency> doGetCurrencyByCode(const std::string& code) const = 0;
    [[nodiscard]] virtual std::vector<Currency> doGetAllCurrencies() const = 0;
};

inline std::optional<Currency> ICurrencyService::getCurrencyByCode(const std::string& code) const { return doGetCurrencyByCode(code); }
inline std::vector<Currency> ICurrencyService::getAllCurrencies() const { return doGetAllCurrencies(); }