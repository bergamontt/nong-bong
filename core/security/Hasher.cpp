#include "Hasher.h"
#include <argon2.h>
#include <stdexcept>
#include <random>

std::string Hasher::hashPin(const std::string& pin) {
    char encoded[128];
    auto salt = generateSalt();
    const int ret = argon2id_hash_encoded(2, 1<<16, 1, pin.c_str(), pin.size(), salt.c_str(), salt.size(), 32, encoded, sizeof(encoded));


    if (ret != ARGON2_OK)
        return pin;

    return std::string(encoded);
}

bool Hasher::verifyPin(const std::string& pin, const std::string& hash) {
    return argon2id_verify(hash.c_str(), pin.c_str(), pin.size()) == ARGON2_OK;
}


std::string Hasher::generateSalt(size_t length) {
    static const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, sizeof(charset) - 2);

    std::string salt;
    salt.reserve(length);
    for (size_t i = 0; i < length; ++i)
        salt += charset[dist(gen)];
    return salt;
}
