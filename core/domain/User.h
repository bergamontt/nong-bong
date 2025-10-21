#pragma once
#include <optional>
#include <string>
#include <ctime>

class User
{
public:
    int id;
    std::string firstName;
    std::string lastName;
    std::optional<std::string> email;
    std::string phone;
    std::optional<std::tm> createdAt;
    std::string passwordHash;
    std::string status;
    int failedLoginCount;
    std::optional<std::tm> blockedUntil;
};
