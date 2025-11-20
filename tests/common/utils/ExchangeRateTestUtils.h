#pragma once
#include "doctest.h"
#include "ExchangeRate.h"

inline void assertExchangeRateEquals(const ExchangeRate& expected, const ExchangeRate& actual)
{
    CHECK_EQ(expected.id, actual.id);
    CHECK_EQ(expected.rate, actual.rate);
    CHECK_EQ(expected.baseCurrency, actual.baseCurrency);
    CHECK_EQ(expected.targetCurrency, actual.targetCurrency);
}