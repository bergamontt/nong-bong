#include "doctest.h"
#include "DBTestFixture.h"
#include "ScheduledTransferDao.h"
#include "ScheduledTransferTestUtils.h"

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
    transfer.description = "Transfer";

    SUBCASE("create should insert valid scheduled transfer")
    {
        CHECK_NOTHROW(dao.create(transfer));
        auto retrieved = dao.getById(transfer.id);
        REQUIRE(retrieved.has_value());
        assertScheduledTransferEquals(*retrieved, transfer);
    }

    SUBCASE("getAllActiveScheduledTransfersBeforeDate should return fitting transfers")
    {
        std::tm pastDate{};
        pastDate.tm_year = 2020 - 1900;
        pastDate.tm_mon = 0;
        pastDate.tm_mday = 1;

        transfer.nextRun = pastDate;
        transfer.active = 1;
        dao.create(transfer);

        std::tm now{};
        now.tm_year = 2025 - 1900;
        now.tm_mon = 10;
        now.tm_mday = 20;

        auto retrieved = dao.getAllActiveBeforeDate(now);
        REQUIRE_EQ(retrieved.size(), 1);
        assertScheduledTransferEquals(transfer, retrieved[0]);
    }

    SUBCASE("getByFromCardId should return correct transfers")
    {
        transfer.fromCardId = 1;
        ScheduledTransfer other = transfer;
        other.fromCardId = 99;

        dao.create(transfer);
        dao.create(other);

        auto retrieved = dao.getByFromCardId(1);
        REQUIRE_EQ(retrieved.size(), 1);
        assertScheduledTransferEquals(transfer, retrieved[0]);
    }

    SUBCASE("update should modify existing transfer in database")
    {
        dao.create(transfer);

        auto retrieved = dao.getById(transfer.id);
        retrieved->amount = 200;
        retrieved->description = "Updated description";
        CHECK_NOTHROW(dao.update(*retrieved));

        auto updated = dao.getById(transfer.id);
        REQUIRE(updated.has_value());
        assertScheduledTransferEquals(*retrieved, *updated);
    }

    SUBCASE("Delete should remove scheduled transfer with the given id")
    {
        dao.create(transfer);
        CHECK_NOTHROW(dao.deleteById(1));
        auto retrieved = dao.getById(transfer.id);
        CHECK_FALSE(retrieved.has_value());
    }

}