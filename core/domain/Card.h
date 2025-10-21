#pragma once
#include <optional>
#include <string>
#include <ctime>

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
