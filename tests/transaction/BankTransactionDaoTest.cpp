#include "doctest.h"
#include "DBTestFixture.h"
#include "BankTransactionDao.h"
#include "BankTransactionTestUtils.h"

TEST_CASE_FIXTURE(DBTestFixture, "BankTransactionDao API test")
{
    const BankTransactionDao dao(pool());

    BankTransaction tx;
    tx.id = 1;
    tx.type = "transfer";
    tx.fromCardId = 1;
    tx.toCardId = 2;
    tx.amount = 100;
    tx.currencyCode = "USD";
    tx.description = "Test transfer";
    tx.status = "completed";

    SUBCASE("create should insert valid bank transaction")
    {
        CHECK_NOTHROW(dao.create(tx));
        auto retrieved = dao.getById(tx.id);
        REQUIRE(retrieved.has_value());
        assertBankTransactionEquals(tx, *retrieved);
    }

    SUBCASE("create with comment should create transaction with comment")
    {
        tx.comment = "Comment";
        CHECK_NOTHROW(dao.create(tx));
        auto retrieved = dao.getById(tx.id);
        REQUIRE(retrieved.has_value());
        assertBankTransactionEquals(tx, *retrieved);
    }

    SUBCASE("Get by fromCardId should return correct transactions")
    {
        BankTransaction tx2 = tx;
        tx2.fromCardId = 1;
        BankTransaction tx3 = tx;
        tx3.fromCardId = 3;

        dao.create(tx);
        dao.create(tx2);
        dao.create(tx3);

        auto retrieved = dao.getByFromCardId(1);
        CHECK_EQ(retrieved.size(), 2);
        for (const auto& r : retrieved)
            CHECK_EQ(r.fromCardId.value(), 1);
    }

    SUBCASE("Get by user card id should return only user's transactions")
    {
        BankTransaction txOther = tx;
        txOther.fromCardId = 99;
        txOther.toCardId = 1;

        dao.create(tx);
        dao.create(txOther);

        auto retrieved = dao.getByUserCardId(1);
        CHECK_EQ(retrieved.size(), 2);
        for (const auto& t : retrieved)
        {
            bool valid = (t.fromCardId.value() == 1) || (t.toCardId.value() == 1);
            CHECK(valid);
        }
    }

}