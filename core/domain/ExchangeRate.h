#pragma once
#include <string>
#include <soci/soci.h>

class ExchangeRate
{
public:
    int id;
    std::string baseCurrency;
    std::string targetCurrency;
    double rate;
};

template <>
struct soci::type_conversion<ExchangeRate>
{
    typedef values base_type;
    static void from_base(values const& v, indicator, ExchangeRate& e)
    {
        e.id = v.get<int>("id");
        e.baseCurrency = v.get<std::string>("base_currency");
        e.targetCurrency = v.get<std::string>("target_currency");
        e.rate = v.get<double>("rate");
    }

    static void to_base(const ExchangeRate& c, values& v, indicator& ind)
    {
        v.set("id", c.id);
        v.set("base_currency", c.baseCurrency);
        v.set("target_currency", c.targetCurrency);
        v.set("rate", c.rate);
    }
};