#pragma once
#include <optional>
#include <string>
#include <ctime>
#include <soci/soci.h>

class ScheduledTransfer
{
public:
    int id;
    int fromCardId;
    int toCardId;
    int amount;
    std::string currencyCode;
    std::string frequency;
    std::optional<std::tm> nextTun;
    int active;
    std::string description;
    std::optional<std::string> comment;
};

template <>
struct soci::type_conversion<ScheduledTransfer>
{
    typedef values base_type;
    static void from_base(values const& v, indicator, ScheduledTransfer& s)
    {
        s.id = v.get<int>("id");
        s.fromCardId = v.get<int>("from_card_id");
        s.toCardId = v.get<int>("to_card_id");
        s.amount = v.get<int>("amount");
        s.currencyCode = v.get<std::string>("currency_code");
        s.frequency = v.get<std::string>("frequency");
        s.nextTun = v.get<std::tm>("next_tun");
        s.active = v.get<int>("active");
        s.description = v.get<std::string>("description");
        s.comment = v.get<std::string>("comment");
    }

    static void to_base(const ScheduledTransfer& s, values& v, indicator& ind)
    {
        v.set("id", s.id);
        v.set("from_card_id", s.fromCardId);
        v.set("to_card_id", s.toCardId);
        v.set("amount", s.amount);
        v.set("currency_code", s.currencyCode);
        v.set("frequency", s.frequency);
        v.set("next_tun", s.nextTun, s.nextTun.has_value() ? i_ok : i_null);
        v.set("active", s.active);
        v.set("description", s.description);
        v.set("comment", s.comment, s.comment.has_value() ? i_ok : i_null);
    }
};