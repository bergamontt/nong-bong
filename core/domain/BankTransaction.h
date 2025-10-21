#pragma once
#include <optional>
#include <string>
#include <ctime>

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
    std::optional<std::string> description;
    std::string status;
};