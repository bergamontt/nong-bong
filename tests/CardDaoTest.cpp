#include "doctest.h"
#include "DBTestFixture.h"
#include "CardDao.h"

TEST_CASE_FIXTURE(DBTestFixture, "CardDao API test")
{
    const CardDao dao(pool());

    Card card;
    card.userId = 1;
    card.cardNumber = "1234567890123456";
    card.allowCredit = 1;
    card.creditLimit = 1000;
    card.currencyCode = "USD";
    card.balance = 500;
    card.dailyLimit = 100;
    card.pinHash = "1234";
    card.status = Card::Status::active;
    card.failedAccessCount = 0;

    SUBCASE("Create should insert valid card")
    {
        CHECK_NOTHROW(dao.create(card));
        auto retrieved = dao.getById(1);
        REQUIRE(retrieved.has_value());
        CHECK_EQ(retrieved->cardNumber, card.cardNumber);
        CHECK_EQ(retrieved->allowCredit, card.allowCredit);
        CHECK_EQ(retrieved->creditLimit, card.creditLimit);
        CHECK_EQ(retrieved->currencyCode, card.currencyCode);
        CHECK_EQ(retrieved->balance, card.balance);
        CHECK_EQ(retrieved->dailyLimit, card.dailyLimit);
        CHECK_EQ(retrieved->designId, card.designId);
        CHECK_EQ(retrieved->pinHash, card.pinHash);
        CHECK_EQ(retrieved->status, card.status);
        CHECK_EQ(retrieved->failedAccessCount, card.failedAccessCount);
        CHECK_EQ(retrieved->blockedUntil, std::nullopt);
    }

    SUBCASE("Get by id should return card with the given id")
    {
        dao.create(card);
        auto retrieved = dao.getById(1);
        REQUIRE(retrieved.has_value());
        CHECK_EQ(retrieved->userId, card.userId);
        CHECK_EQ(retrieved->currencyCode, card.currencyCode);
    }

    SUBCASE("Get by user id should return all cards for a user")
    {
        dao.create(card);
        auto cards = dao.getByUserId(1);
        CHECK_EQ(cards.size(), 1);
        CHECK_EQ(cards[0].userId, card.userId);
    }

    SUBCASE("Get by user and status should return fitting cards")
    {
        dao.create(card);
        auto activeCards = dao.getByUserIdAndStatus(1, Card::Status::active);
        CHECK_EQ(activeCards.size(), 1);
    }

    SUBCASE("Get by user and status should return empty vector if no fitting cards found")
    {
        card.status = Card::Status::blocked;
        auto activeCards = dao.getByUserIdAndStatus(1, Card::Status::active);
        CHECK_EQ(activeCards.size(), 0);
    }

    SUBCASE("Update should save modified data")
    {
        dao.create(card);
        auto retrieved = dao.getById(1);
        REQUIRE(retrieved.has_value());
        retrieved->balance = 999;
        retrieved->status = Card::Status::blocked;
        CHECK_NOTHROW(dao.update(*retrieved));
        auto updated = dao.getById(1);
        REQUIRE(updated.has_value());
        CHECK_EQ(updated->balance, 999);
        CHECK_EQ(updated->status, Card::Status::blocked);
    }

    SUBCASE("UpdatePin should save new pin")
    {
        dao.create(card);
        auto retrieved = dao.getById(1);
        REQUIRE(retrieved.has_value());
        retrieved->pinHash = "4321";
        CHECK_NOTHROW(dao.updatePin(*retrieved));
        auto updated = dao.getById(1);
        REQUIRE(updated.has_value());
        CHECK_EQ(updated->pinHash, "4321");
    }

}