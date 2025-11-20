#pragma once
#include "doctest.h"
#include "User.h"
#include "Hasher.h"

static void assertUserBase(const User& expected, const User& actual)
{
    CHECK_EQ(expected.id, actual.id);
    CHECK_EQ(expected.firstName, actual.firstName);
    CHECK_EQ(expected.lastName, actual.lastName);
    CHECK_EQ(expected.email, actual.email);
    CHECK_EQ(expected.phone, actual.phone);
    CHECK_EQ(expected.status, actual.status);
    CHECK_EQ(expected.failedLoginCount, actual.failedLoginCount);
}

inline void assertUserEqualsHashed(const User& expected, const User& actual)
{
    assertUserBase(expected, actual);
    CHECK(Hasher::verifyPin(expected.passwordHash, actual.passwordHash));
}

inline void assertUserEquals(const User& expected, const User& actual)
{
    assertUserBase(expected, actual);
    CHECK_EQ(actual.passwordHash, expected.passwordHash);
}