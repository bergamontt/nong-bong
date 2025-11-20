#pragma once
#include "doctest.h"
#include "Card.h"
#include "Hasher.h"

static void assertCardBase(const Card& expected, const Card& actual) {
    CHECK_EQ(actual.id, expected.id);
    CHECK_EQ(actual.userId, expected.userId);
    CHECK_EQ(actual.cardNumber, expected.cardNumber);
    CHECK_EQ(actual.allowCredit, expected.allowCredit);
    CHECK_EQ(actual.creditLimit, expected.creditLimit);
    CHECK_EQ(actual.currencyCode, expected.currencyCode);
    CHECK_EQ(actual.balance, expected.balance);
    CHECK_EQ(actual.dailyLimit, expected.dailyLimit);
    CHECK_EQ(actual.designId, expected.designId);
    CHECK_EQ(actual.status, expected.status);
    CHECK_EQ(actual.failedAccessCount, expected.failedAccessCount);
}

inline void assertCardEqualsHashed(const Card& expected, const Card& actual) {
    assertCardBase(expected, actual);
    CHECK(Hasher::verifyPin(expected.pinHash, actual.pinHash));
}

inline void assertCardEquals(const Card& expected, const Card& actual) {
    assertCardBase(expected, actual);
    CHECK_EQ(actual.pinHash, expected.pinHash);
}

inline Card makeCard(const int id, std::string currency, const int balance)
{
    Card c;
    c.id = id;
    c.userId = id;
    c.cardNumber = std::to_string(id);
    c.allowCredit = true;
    c.creditLimit = 50000;
    c.currencyCode = std::move(currency);
    c.balance = balance;
    c.dailyLimit = 999999;
    c.designId = 1;
    c.pinHash = "hash";
    c.status = Card::active;
    c.failedAccessCount = 0;
    return c;
}
