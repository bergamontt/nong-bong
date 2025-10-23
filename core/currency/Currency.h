#pragma once
#include <string>
#include <soci/soci.h>

class Currency
{
public:
    std::string code;
    std::string name;
    int minorUnit;
};

template <>
struct soci::type_conversion<Currency>
{
    typedef values base_type;
    static void from_base(values const& v, indicator, Currency& c)
    {
        c.code = v.get<std::string>("code");
        c.name = v.get<std::string>("name");
        c.minorUnit = v.get<int>("minor_unit");
    }

    static void to_base(const Currency& c, values& v, indicator& ind)
    {
        v.set("code", c.code);
        v.set("name", c.name);
        v.set("minor_unit", c.minorUnit);
    }
};