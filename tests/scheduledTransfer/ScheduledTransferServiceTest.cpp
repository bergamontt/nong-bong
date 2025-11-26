#include "doctest.h"
#include "DBTestFixture.h"
#include "ScheduledTransferDao.h"
#include "ScheduledTransferService.h"
#include "BankTransactionDao.h"
#include "BankTransactionService.h"
#include "CardDao.h"
#include "CurrencyDao.h"
#include "ExchangeRateDao.h"
#include "ScheduledTransferTestUtils.h"
#include "CardTestUtils.h"
#include "CurrencyService.h"
#include "CardService.h"

TEST_CASE_FIXTURE(DBTestFixture, "ScheduledTransferService API Integration Test")
{
    ScheduledTransferDao scheduledDao(pool());
    BankTransactionDao txDao(pool());
    CardDao cardDao(pool());
    CurrencyDao currencyDao(pool());
    ExchangeRateDao rateDao(pool());
    CurrencyService currencyService(currencyDao, rateDao);
    CardService cardService(cardDao, txDao, rateDao);

    BankTransactionService bankTxService(txDao, cardService, currencyService);
    ScheduledTransferService service(scheduledDao, txDao, bankTxService,currencyService);

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
    rateDao.create(er);

    Card c1 = makeCard(1, "USD", 100000);
    Card c2 = makeCard(2, "USD", 50000);
    cardDao.create(c1);
    cardDao.create(c2);

    ScheduledTransfer tr;
    tr.id = 1;
    tr.fromCardId = 1;
    tr.toCardId = 2;
    tr.amount = 100;
    tr.currencyCode = "USD";
    tr.frequency = "daily";
    tr.active = 1;
    tr.description = "Test scheduled";
    tr.comment = "Comment";

    std::tm now{};
    now.tm_year = 127;
    now.tm_mon = 0;
    now.tm_mday = 1;

    tr.nextTun = now;

    SUBCASE("createScheduledTransfer should insert valid transfer into database")
    {
        CHECK_NOTHROW(service.createScheduledTransfer(tr));
        auto retrieved = service.getScheduledTransferById(tr.id);
        REQUIRE(retrieved.has_value());
        assertScheduledTransferEquals(tr, *retrieved);
    }

    SUBCASE("getScheduledTransferById should return valid transfer with given id if such transfer exists")
    {
        service.createScheduledTransfer(tr);
        auto retrieved = service.getScheduledTransferById(tr.id);
        REQUIRE(retrieved.has_value());
        assertScheduledTransferEquals(tr, *retrieved);
    }

    SUBCASE("getScheduledTransferById should return no value unless such transfer exists")
    {
        auto retrieved = service.getScheduledTransferById(tr.id);
        CHECK_FALSE(retrieved.has_value());
    }

    SUBCASE("updateScheduledTransfer should modify existing transfer in database")
    {
        service.createScheduledTransfer(tr);

        ScheduledTransfer updated = tr;
        updated.amount = 200;
        updated.frequency = "weekly";
        updated.description = "Updated desc";
        updated.comment = "Updated comment";

        CHECK_NOTHROW(service.updateScheduledTransfer(updated));

        auto retrieved = service.getScheduledTransferById(tr.id);
        REQUIRE(retrieved.has_value());
        assertScheduledTransferEquals(updated, *retrieved);
    }

    SUBCASE("deleteScheduledTransfer should remove transfer with given id from database")
    {
        service.createScheduledTransfer(tr);
        CHECK_NOTHROW(service.deleteScheduledTransfer(tr.id));
        auto retrieved = service.getScheduledTransferById(tr.id);
        CHECK_FALSE(retrieved.has_value());
    }

    SUBCASE("getAllScheduledTransfersFromCardId should return all transfers with given card id")
    {
        service.createScheduledTransfer(tr);
        auto retrieved = service.getAllScheduledTransfersFromCardId(tr.fromCardId);
        REQUIRE_EQ(retrieved.size(), 1);
        assertScheduledTransferEquals(tr, retrieved[0]);
    }

    SUBCASE("executeAllScheduledTransfersByNow should create BankTransaction for due transfers")
    {
        service.createScheduledTransfer(tr);
        service.executeAllScheduledTransfersByNow(now);

        auto transactions = bankTxService.getAllBankTransactionsFromCardId(tr.fromCardId);
        REQUIRE_FALSE(transactions.empty());

        const BankTransaction& tx = transactions.back();

        CHECK_EQ(tx.fromCardId.value(), tr.fromCardId);
        CHECK_EQ(tx.toCardId.value(), tr.toCardId);
        CHECK_EQ(tx.amount, tr.amount * usd.minorUnit);
        CHECK_EQ(tx.currencyCode, tr.currencyCode);
        //CHECK_EQ(tx.status, "completed");

        auto retrieved = service.getScheduledTransferById(tr.id);
        REQUIRE(retrieved.has_value());
        CHECK(retrieved->nextTun.has_value());
        CHECK_EQ(retrieved->nextTun->tm_mday, now.tm_mday + 1);
    }

}