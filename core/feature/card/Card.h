#pragma once
#include <optional>
#include <string>
#include <ctime>
#include <soci/soci.h>

class Card
{
public:

    enum Status
    {
        active,
        blocked,
        deleted
    };

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
    Status status;
    int failedAccessCount;
    std::optional<std::tm> blockedUntil;
};

inline std::string statusToString(const Card::Status status)
{
    switch (status)
    {
        case Card::Status::active: return "active";
        case Card::Status::blocked: return "blocked";
        case Card::Status::deleted: return "deleted";
    }
    return {};
}

inline Card::Status statusFromString(const std::string& status)
{
    if (status == "active")
        return Card::Status::active;
    if (status == "blocked")
        return Card::Status::blocked;
    if (status == "deleted")
        return Card::Status::deleted;
    throw std::invalid_argument("Invalid status");
}

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
        c.status = statusFromString(v.get<std::string>("status"));
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
        v.set("status", statusToString(c.status));
        v.set("failed_access_count", c.failedAccessCount);
        v.set("blocked_until", c.blockedUntil, c.blockedUntil.has_value() ? i_ok : i_null);
    }
};