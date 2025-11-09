#pragma once
#include <optional>
#include <string>
#include <soci/soci.h>

class CardDesign
{
public:
    int id;
    std::string name;
    std::optional<std::string> author;
    std::string imageRef;
};

template <>
struct soci::type_conversion<CardDesign>
{
    typedef values base_type;
    static void from_base(values const& v, indicator, CardDesign& c)
    {
        c.id = v.get<int>("id");
        c.name = v.get<std::string>("name");
        c.author = (v.get_indicator("author") == i_null)
                  ? std::nullopt
                    : std::make_optional(v.get<std::string>("author"));
        c.imageRef = v.get<std::string>("image_ref");
    }

    static void to_base(const CardDesign& c, values& v, indicator& ind)
    {
        v.set("id", c.id);
        v.set("name", c.name);
        v.set("author", c.author, c.author.has_value() ? i_ok : i_null);
        v.set("image_ref", c.imageRef);
    }
};