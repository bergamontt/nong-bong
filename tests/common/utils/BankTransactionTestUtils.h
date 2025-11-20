#pragma once
#include "BankTransaction.h"
#include "doctest.h"

inline void assertBankTransactionEquals(const BankTransaction& expected, const BankTransaction& actual)
{
    CHECK_EQ(expected.id, actual.id);
    CHECK_EQ(expected.type, actual.type);
    CHECK_EQ(expected.fromCardId, actual.fromCardId);
    CHECK_EQ(expected.toCardId, actual.toCardId);
    CHECK_EQ(expected.amount, actual.amount);
    CHECK_EQ(expected.currencyCode, actual.currencyCode);
    CHECK_EQ(expected.description, actual.description);
    CHECK_EQ(expected.comment, actual.comment);
    CHECK_EQ(expected.status, actual.status);
}