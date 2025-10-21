#pragma once
#include <optional>
#include <string>
#include <ctime>

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
};