#pragma once
#include <optional>
#include <string>
#include <ctime>
#include <soci/soci.h>

class Card
{
public:
    int id;
    int userId;
    std::string cardNumber;
    int allowCredit;
    std::optional<int> creditLimit;
    std::string currencyCode;
    int balance;
    int dailyLimit;
    std::optional<int> designId;
    std::string pinHash;
    std::string status;
    int failedAccessCount;
    std::optional<std::tm> blockedUntil;
};

template <>
struct soci::type_conversion<Card>
{
    typedef values base_type;
    static void from_base(values const& v, indicator, Card& c)
    {
        c.id = v.get<int>("id");
        c.userId = v.get<int>("user_id");
        c.cardNumber = v.get<std::string>("card_number");
        c.allowCredit = v.get<int>("allow_credit");
        c.creditLimit = v.get<int>("credit_limit");
        c.currencyCode = v.get<std::string>("currency_code");
        c.balance = v.get<int>("balance");
        c.dailyLimit = v.get<int>("daily_limit");
        c.designId = v.get<int>("design_id");
        c.pinHash = v.get<std::string>("pin_hash");
        c.status = v.get<std::string>("status");
        c.failedAccessCount = v.get<int>("failed_access_count");
        c.blockedUntil = v.get<std::tm>("blocked_until");
    }

    static void to_base(const Card& c, values& v, indicator& ind)
    {
        v.set("id", c.id);
        v.set("user_id", c.userId);
        v.set("card_number", c.cardNumber);
        v.set("allow_credit", c.allowCredit);
        v.set("credit_limit", c.creditLimit, c.creditLimit.has_value() ? i_ok : i_null);
        v.set("currency_code", c.currencyCode);
        v.set("balance", c.balance);
        v.set("daily_limit", c.dailyLimit);
        v.set("design_id", c.designId, c.designId.has_value() ? i_ok : i_null);
        v.set("pin_hash", c.pinHash);
        v.set("status", c.status);
        v.set("failed_access_count", c.failedAccessCount);
        v.set("blocked_until", c.blockedUntil, c.blockedUntil.has_value() ? i_ok : i_null);
    }
};