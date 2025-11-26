#include "doctest.h"
#include "DBTestFixture.h"
#include "BankTransactionDao.h"
#include "BankTransactionService.h"
#include "CardDao.h"
#include "ExchangeRateDao.h"
#include "CurrencyDao.h"
#include "BankTransactionTestUtils.h"
#include "CardTestUtils.h"
#include "CardService.h"
#include "CurrencyService.h"

TEST_CASE_FIXTURE(DBTestFixture, "BankTransactionService API Integration Test")
{
    BankTransactionDao txDao(pool());
    CardDao cardDao(pool());
    ExchangeRateDao exchangeRateDao(pool());
    CurrencyDao currencyDao(pool());
    CurrencyService currencyService(currencyDao, exchangeRateDao);
    CardService cardService(cardDao, txDao, exchangeRateDao);

    BankTransactionService service(
        txDao, cardService,
        currencyService
    );

    Currency usd;
    usd.code = "USD";
    usd.name = "US Dollar";
    usd.minorUnit = 100;
    currencyDao.create(usd);

    ExchangeRate er;
    er.id = 1;
    er.baseCurrency = "USD";
    er.targetCurrency = "USD";
    er.rate = 1.0;
    exchangeRateDao.create(er);

    Card c1 = makeCard(1, "USD", 100000);
    Card c2 = makeCard(2, "USD", 50000);
    cardDao.create(c1);
    cardDao.create(c2);

    BankTransaction tx;
    tx.id = 1;
    tx.type = "transfer";
    tx.fromCardId = 1;
    tx.toCardId = 2;
    tx.amount = 100;
    tx.currencyCode = "USD";
    tx.description = "Test transfer";
    tx.comment = "Comment";
    tx.status = "completed";

    SUBCASE("createBankTransaction should insert valid transaction into database")
    {
        CHECK(service.createBankTransaction(tx));
        auto retrieved = service.getBankTransactionById(tx.id);
        CHECK(retrieved.has_value());
        assertBankTransactionEquals(tx, *retrieved);
    }

    SUBCASE("getBankTransactionById should return transaction with given id if such record exists")
    {
        service.createBankTransaction(tx);
        auto retrieved = service.getBankTransactionById(tx.id);
        CHECK(retrieved.has_value());
        assertBankTransactionEquals(tx, *retrieved);
    }

    SUBCASE("getBankTransactionById should return no value unless such record exists")
    {
        auto retrieved = service.getBankTransactionById(tx.id);
        CHECK_FALSE(retrieved.has_value());
    }

    SUBCASE("getAllBankTransactionsFromCardId should return all transactions with given from card id")
    {
        BankTransaction tx2 = tx;
        tx2.fromCardId = 2;

        service.createBankTransaction(tx);
        service.createBankTransaction(tx2);

        auto retrieved = service.getAllBankTransactionsFromCardId(tx.fromCardId.value());
        REQUIRE_EQ(retrieved.size(), 1);
        assertBankTransactionEquals(tx, retrieved[0]);
    }

    SUBCASE("getBankTransactionByUserCardId should return incoming and outgoing transactions with given user id")
    {
        BankTransaction tx2 = tx;
        tx2.id = 2;
        tx2.fromCardId = 2;
        tx2.toCardId = 1;

        service.createBankTransaction(tx);
        service.createBankTransaction(tx2);

        auto result = service.getBankTransactionByUserCardId(c1.id);
        REQUIRE_EQ(result.size(), 2);

        assertBankTransactionEquals(tx, result[0]);
        assertBankTransactionEquals(tx2, result[1]);
    }

}