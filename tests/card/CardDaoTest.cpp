#include "doctest.h"
#include "DBTestFixture.h"
#include "EntityTestUtils.h"
#include "CardDao.h"

TEST_CASE_FIXTURE(DBTestFixture, "CardDao API test")
{
    const CardDao dao(pool());

    Card card;
    card.id = 1;
    card.userId = 1;
    card.cardNumber = "1234567890123456";
    card.allowCredit = 1;
    card.creditLimit = 1000;
    card.currencyCode = "USD";
    card.balance = 500;
    card.dailyLimit = 100;
    card.designId = 1;
    card.pinHash = "1234";
    card.status = Card::Status::active;
    card.failedAccessCount = 0;

    SUBCASE("create should insert valid card")
    {
        CHECK_NOTHROW(dao.create(card));
        auto retrieved = dao.getById(card.id);
        REQUIRE(retrieved.has_value());
        assertCardEquals(card, *retrieved);
    }

    SUBCASE("getById should return card with the given id")
    {
        dao.create(card);
        auto retrieved = dao.getById(card.id);
        REQUIRE(retrieved.has_value());
        assertCardEquals(card, *retrieved);
    }

    SUBCASE("getByUserId should return all cards for a user")
    {
        dao.create(card);
        auto retrieved = dao.getByUserId(card.id);
        CHECK_EQ(retrieved.size(), 1);
        assertCardEquals(card, retrieved[0]);
    }

    SUBCASE("getByUserAndStatus should return fitting cards")
    {
        dao.create(card);
        auto activeCards = dao.getByUserIdAndStatus(card.userId, Card::Status::active);
        CHECK_EQ(activeCards.size(), 1);
        assertCardEquals(card, activeCards[0]);
    }

    SUBCASE("getByUserAndStatus should return empty vector if no fitting cards found")
    {
        card.status = Card::Status::blocked;
        auto activeCards = dao.getByUserIdAndStatus(card.userId, Card::Status::active);
        CHECK(activeCards.empty());
    }

    SUBCASE("update should save modified data in database")
    {
        dao.create(card);
        auto retrieved = dao.getById(card.id);
        REQUIRE(retrieved.has_value());
        retrieved->balance = 999;
        retrieved->status = Card::Status::blocked;
        CHECK_NOTHROW(dao.update(*retrieved));
        auto updated = dao.getById(card.id);
        REQUIRE(updated.has_value());
        assertCardEquals(*retrieved, *updated);
    }

    SUBCASE("updatePin should save new pin")
    {
        dao.create(card);
        auto retrieved = dao.getById(card.id);
        REQUIRE(retrieved.has_value());
        retrieved->pinHash = "4321";
        CHECK_NOTHROW(dao.updatePin(*retrieved));
        auto updated = dao.getById(card.id);
        REQUIRE(updated.has_value());
        CHECK_EQ(updated->pinHash, retrieved->pinHash);
    }

}