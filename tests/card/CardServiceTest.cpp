#include "doctest.h"
#include "DBTestFixture.h"
#include "CardTestUtils.h"
#include "CardDao.h"
#include "BankTransactionDao.h"
#include "ExchangeRateDao.h"
#include "CardService.h"
#include "Hasher.h"

TEST_CASE_FIXTURE(DBTestFixture, "CardService API Integration Test")
{
    CardDao cardDao(pool());
    BankTransactionDao transDao(pool());
    ExchangeRateDao rateDao(pool());
    CardService service(cardDao, transDao, rateDao);

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

    SUBCASE("createCard should insert valid card with optional fields")
    {
        CHECK_NOTHROW(service.createCard(card));
        auto retrieved = service.getCardById(card.id);
        REQUIRE(retrieved.has_value());
        assertCardEqualsHashed(card, *retrieved);
    }

    SUBCASE("createCard should insert valid card without optional fields")
    {
        card.creditLimit.reset();
        card.designId.reset();
        CHECK_NOTHROW(service.createCard(card));
        auto retrieved = service.getCardById(card.id);
        REQUIRE(retrieved.has_value());
        assertCardEqualsHashed(card, *retrieved);
    }

    SUBCASE("getCardById should return valid card with given id if such card exists")
    {
        service.createCard(card);
        auto retrieved = service.getCardById(card.id);
        REQUIRE(retrieved.has_value());
        assertCardEqualsHashed(card, *retrieved);
    }

    SUBCASE("getCardById should return no value unless such card exist")
    {
        auto retrieved = service.getCardById(card.id);
        CHECK_FALSE(retrieved.has_value());
    }

    SUBCASE("getCardByNumber should return card with given number if such card exists")
    {
        service.createCard(card);
        auto retrieved = service.getCardByNumber(card.cardNumber);
        REQUIRE(retrieved.has_value());
        assertCardEqualsHashed(card, *retrieved);
    }

    SUBCASE("getCardByNumber should return no value unless such card exists")
    {
        auto retrieved = service.getCardByNumber(card.cardNumber);
        CHECK_FALSE(retrieved.has_value());
    }

    SUBCASE("getAllActiveCardsByUserId should return only active cards")
    {
        Card card2 = card;
        card2.id = 2;
        card2.cardNumber = "123";
        card2.status = Card::Status::blocked;

        Card card3 = card;
        card3.id = 3;
        card3.cardNumber = "456";
        card3.status = Card::Status::deleted;

        service.createCard(card);
        service.createCard(card2);
        service.createCard(card3);

        auto retrieved = service.getAllActiveCardsByUserId(card.userId);
        CHECK_EQ(retrieved.size(), 1);
        assertCardEqualsHashed(card, retrieved[0]);
    }

    SUBCASE("getAllBlockedCardsByUserId should return only blocked cards")
    {
        Card card2 = card;
        card2.id = 2;
        card2.cardNumber = "123";
        card2.status = Card::Status::blocked;

        Card card3 = card;
        card3.id = 3;
        card3.cardNumber = "456";
        card3.status = Card::Status::deleted;

        service.createCard(card);
        service.createCard(card2);
        service.createCard(card3);

        auto retrieved = service.getAllBlockedCardsByUserId(card.userId);
        CHECK_EQ(retrieved.size(), 1);
        assertCardEqualsHashed(card2, retrieved[0]);
    }

    SUBCASE("getAllDeletedCardsByUserId should return only deleted cards")
    {
        Card card2 = card;
        card2.id = 2;
        card2.cardNumber = "123";
        card2.status = Card::Status::blocked;

        Card card3 = card;
        card3.id = 3;
        card3.cardNumber = "456";
        card3.status = Card::Status::deleted;

        service.createCard(card);
        service.createCard(card2);
        service.createCard(card3);

        auto retrieved = service.getAllDeletedCardsByUserId(card.userId);
        CHECK_EQ(retrieved.size(), 1);
        assertCardEqualsHashed(card3, retrieved[0]);
    }

    SUBCASE("getAllCardsByUserId should return all user cards regardless of status")
    {
        Card card2 = card;
        card2.id = 2;
        card2.cardNumber = "123";
        card2.status = Card::Status::blocked;

        Card card3 = card;
        card3.id = 3;
        card3.cardNumber = "456";
        card3.status = Card::Status::deleted;

        service.createCard(card);
        service.createCard(card2);
        service.createCard(card3);

        auto retrieved = service.getAllCardsByUserId(card.userId);
        CHECK_EQ(retrieved.size(), 3);
        assertCardEqualsHashed(card, retrieved[0]);
        assertCardEqualsHashed(card2, retrieved[1]);
        assertCardEqualsHashed(card3, retrieved[2]);
    }

    SUBCASE("updateCard should modify existing card in database")
    {
        service.createCard(card);

        card.cardNumber = "123";
        card.balance = 500;
        card.pinHash = "1234";
        card.status = Card::Status::blocked;

        CHECK_NOTHROW(service.updateCard(card));
        auto retrieved = service.getCardById(card.id);
        REQUIRE(retrieved.has_value());
        assertCardEqualsHashed(card, *retrieved);
    }

    SUBCASE("deleteCardById should set card status to deleted")
    {
        service.createCard(card);
        CHECK_NOTHROW(service.deleteCardById(card.id));
        auto retrieved = service.getCardById(card.id);
        REQUIRE(retrieved.has_value());
        card.status = Card::Status::deleted;
        CHECK_NOTHROW(service.deleteCardById(card.id));
    }

    SUBCASE("accessToCard should return true for correct pin")
    {
        service.createCard(card);
        CHECK(service.accessToCard(card.id, card.pinHash));
    }

    SUBCASE("accessToCard should return false for wrong pin")
    {
        service.createCard(card);
        CHECK_FALSE(service.accessToCard(card.id, "0000"));
    }

    SUBCASE("changeCardPin should return true and update PIN if old pin matches")
    {
        service.createCard(card);
        std::string newPin = "0000";
        CHECK(service.changeCardPin(card.id, card.pinHash, newPin));
        auto retrieved = service.getCardById(card.id);
        REQUIRE(retrieved.has_value());
        CHECK(Hasher::verifyPin(newPin, retrieved->pinHash));
    }

    SUBCASE("changeCardPin should return false when old pin does not match")
    {
        service.createCard(card);
        CHECK_FALSE(service.changeCardPin(card.id, "0000", "1111"));
    }

}
