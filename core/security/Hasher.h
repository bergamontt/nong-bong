#pragma once
#include <string>

class Hasher {
public:
    static std::string hashPin(const std::string& pin);
    static bool verifyPin(const std::string& pin, const std::string& hash);
    static std::string generateSalt(size_t length = 16);
};