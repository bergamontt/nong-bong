#include "doctest.h"
#include "DBTestFixture.h"
#include "ScheduledTransferDao.h"

TEST_CASE_FIXTURE(DBTestFixture, "ScheduledTransferDao API test")
{
    const ScheduledTransferDao dao(pool());

    ScheduledTransfer transfer;
    transfer.id = 1;
    transfer.fromCardId = 1;
    transfer.toCardId = 2;
    transfer.amount = 100;
    transfer.currencyCode = "USD";
    transfer.frequency = "weekly";
    transfer.active = 1;
    transfer.description = "Test transfer";

    SUBCASE("create should insert valid scheduled transfer")
    {
        CHECK_NOTHROW(dao.create(transfer));
        auto retrieved = dao.getById(transfer.id);
        CHECK(retrieved.has_value());
        CHECK_EQ(retrieved->fromCardId, transfer.fromCardId);
        CHECK_EQ(retrieved->toCardId, transfer.toCardId);
        CHECK_EQ(retrieved->amount, transfer.amount);
        CHECK_EQ(retrieved->currencyCode, transfer.currencyCode);
        CHECK_EQ(retrieved->frequency, transfer.frequency);
        CHECK_EQ(retrieved->active, transfer.active);
        CHECK_EQ(retrieved->description, transfer.description);
        CHECK_FALSE(retrieved->comment.has_value());
    }

    SUBCASE("getAllActiveScheduledTransfersBeforeDate should return fitting transfers")
    {
        std::tm pastDate{};
        pastDate.tm_year = 2020 - 1900;
        pastDate.tm_mon = 0;
        pastDate.tm_mday = 1;

        transfer.nextTun = pastDate;
        transfer.active = 1;
        dao.create(transfer);

        std::tm now{};
        now.tm_year = 2025 - 1900;
        now.tm_mon = 10;
        now.tm_mday = 20;

        auto retrieved = dao.getAllActiveBeforeDate(now);
        CHECK_FALSE(retrieved.empty());
        REQUIRE_FALSE(retrieved.empty());
        CHECK_EQ(retrieved.size(), 1);
        CHECK_EQ(retrieved[0].fromCardId, 1);
        CHECK_EQ(retrieved[0].active, 1);
    }

    SUBCASE("getByFromCardId should return correct transfers")
    {
        transfer.fromCardId = 1;
        ScheduledTransfer other = transfer;
        other.fromCardId = 99;

        dao.create(transfer);
        dao.create(other);

        auto retrieved = dao.getByFromCardId(1);
        CHECK_EQ(retrieved.size(), 1);
        CHECK_EQ(retrieved[0].fromCardId, 1);
    }

    SUBCASE("update should modify existing transfer in database")
    {
        dao.create(transfer);

        auto retrieved = dao.getById(transfer.id);
        retrieved->amount = 200;
        retrieved->description = "Updated description";
        CHECK_NOTHROW(dao.update(*retrieved));

        auto updated = dao.getById(transfer.id);
        CHECK(updated.has_value());
        CHECK_EQ(updated->amount, retrieved->amount);
        CHECK_EQ(updated->description, retrieved->description);
    }

    SUBCASE("Delete should remove scheduled transfer with the given id")
    {
        dao.create(transfer);
        CHECK_NOTHROW(dao.deleteById(1));
        auto retrieved = dao.getById(transfer.id);
        CHECK_FALSE(retrieved.has_value());
    }

}