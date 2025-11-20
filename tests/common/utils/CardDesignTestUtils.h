#pragma once
#include "doctest.h"
#include "CardDesign.h"

inline void assertCardDesignEquals(const CardDesign& expected, const CardDesign& actual)
{
    CHECK_EQ(expected.id, actual.id);
    CHECK_EQ(expected.name, actual.name);
    CHECK_EQ(expected.author, actual.author);
    CHECK_EQ(expected.imageRef, actual.imageRef);
}