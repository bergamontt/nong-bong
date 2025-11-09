#pragma once
#include <optional>
#include <string>
#include <ctime>
#include <soci/soci.h>

class BankTransaction
{
public:
    int id;
    std::tm createdAt;
    std::string type;
    std::optional<int> fromCardId;
    std::optional<int> toCardId;
    int amount;
    std::string currencyCode;
    std::string description;
    std::optional<std::string> comment;
    std::string status;
};

template <>
struct soci::type_conversion<BankTransaction>
{
    typedef values base_type;
    static void from_base(values const& v, indicator, BankTransaction& bt)
    {
        bt.id = v.get<int>("id");
        bt.createdAt = v.get<std::tm>("created_at");
        bt.type = v.get<std::string>("type");
        bt.fromCardId = (v.get_indicator("from_card_id") == i_null)
            ? std::nullopt
            : std::make_optional(v.get<int>("from_card_id"));
        bt.toCardId = (v.get_indicator("to_card_id") == i_null)
            ? std::nullopt
            : std::make_optional(v.get<int>("to_card_id"));
        bt.amount = v.get<int>("amount");
        bt.currencyCode = v.get<std::string>("currency_code");
        bt.description = v.get<std::string>("description");
        bt.comment = (v.get_indicator("comment") == i_null)
            ? std::nullopt
            : std::make_optional(v.get<std::string>("comment"));
        bt.status = v.get<std::string>("status");
    }

    static void to_base(const BankTransaction& c, values& v, indicator& ind)
    {
        v.set("id", c.id);
        v.set("created_at", c.createdAt);
        v.set("type", c.type);
        v.set("from_card_id", c.fromCardId, c.fromCardId.has_value()? i_ok:i_null);
        v.set("to_card_id", c.toCardId, c.toCardId.has_value()? i_ok:i_null);
        v.set("amount", c.amount);
        v.set("currency_code", c.currencyCode);
        v.set("description", c.description);
        v.set("comment", c.comment, c.comment.has_value()? i_ok:i_null);
        v.set("status", c.status);
    }
};