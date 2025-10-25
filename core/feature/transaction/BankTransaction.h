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
    static void from_base(values const& v, indicator, BankTransaction& c)
    {
        c.id = v.get<int>("id");
        c.createdAt = v.get<std::tm>("created_at");
        c.type = v.get<std::string>("type");
        c.fromCardId = v.get<int>("from_card_id");
        c.toCardId = v.get<int>("to_card_id");
        c.amount = v.get<int>("amount");
        c.currencyCode = v.get<std::string>("currency_code");
        c.description = v.get<std::string>("description");
        c.comment = v.get<std::string>("comment");
        c.status = v.get<std::string>("status");
    }

    static void to_base(const BankTransaction& c, values& v, indicator& ind)
    {
        v.set("id", c.id);
        v.set("created_at", c.createdAt);
        v.set("type", c.type);
        v.set("from_card_id", c.fromCardId, c.fromCardId.has_value() ? i_ok : i_null);
        v.set("to_card_id", c.toCardId, c.toCardId.has_value() ? i_ok : i_null);
        v.set("amount", c.amount);
        v.set("currency_code", c.currencyCode);
        v.set("description", c.description);
        v.set("comment", c.comment, c.comment.has_value() ? i_ok : i_null);
        v.set("status", c.status);
    }
};