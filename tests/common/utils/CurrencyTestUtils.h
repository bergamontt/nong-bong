#pragma once
#include "Currency.h"
#include "doctest.h"

inline void assertCurrencyEquals(const Currency& expected, const Currency& actual)
{
    CHECK_EQ(expected.code, actual.code);
    CHECK_EQ(expected.name, actual.name);
    CHECK_EQ(expected.minorUnit, actual.minorUnit);
}