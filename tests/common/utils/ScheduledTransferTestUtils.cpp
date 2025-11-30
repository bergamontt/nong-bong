#include "EntityTestUtils.h"
#include "doctest.h"
#include "ScheduledTransfer.h"

void assertScheduledTransferEquals(const ScheduledTransfer& expected, const ScheduledTransfer& actual)
{
    CHECK_EQ(expected.id, actual.id);
    CHECK_EQ(expected.fromCardId, actual.fromCardId);
    CHECK_EQ(expected.toCardId, actual.toCardId);
    CHECK_EQ(expected.amount, actual.amount);
    CHECK_EQ(expected.currencyCode, actual.currencyCode);
    CHECK_EQ(expected.frequency, actual.frequency);
    CHECK_EQ(expected.active, actual.active);
    CHECK_EQ(expected.description, actual.description);
    CHECK_EQ(expected.comment, actual.comment);
}