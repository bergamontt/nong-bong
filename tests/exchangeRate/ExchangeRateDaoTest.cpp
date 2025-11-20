#include "doctest.h"
#include "DBTestFixture.h"
#include "ExchangeRateDao.h"
#include "ExchangeRateTestUtils.h"

TEST_CASE_FIXTURE(DBTestFixture, "ExchangeRateDao API test")
{
    const ExchangeRateDao dao(pool());

    ExchangeRate rate1;
    rate1.id = 1;
    rate1.baseCurrency = "USD";
    rate1.targetCurrency = "EUR";
    rate1.rate = 0.93;

    ExchangeRate rate2;
    rate2.id = 2;
    rate2.baseCurrency = "EUR";
    rate2.targetCurrency = "USD";
    rate2.rate = 1.08;

    SUBCASE("create should insert valid exchange rate")
    {
        CHECK_NOTHROW(dao.create(rate1));
        auto retrieved = dao.getById(rate1.id);
        REQUIRE(retrieved.has_value());
        assertExchangeRateEquals(rate1, *retrieved);
    }

    SUBCASE("getById should return valid exchange rate with given id if such rate exists")
    {
        dao.create(rate1);
        auto retrieved = dao.getById(rate1.id);
        REQUIRE(retrieved.has_value());
        assertExchangeRateEquals(rate1, *retrieved);
    }

    SUBCASE("getById should return no value unless such rate exists")
    {
        auto retrieved = dao.getById(rate1.id);
        CHECK_FALSE(retrieved.has_value());
    }

    SUBCASE("getAll should return all existing exchange rates")
    {
        dao.create(rate1);
        dao.create(rate2);
        auto retrieved = dao.getAll();
        REQUIRE_EQ(retrieved.size(), 2);
        assertExchangeRateEquals(rate1, retrieved[0]);
        assertExchangeRateEquals(rate2, retrieved[1]);
    }

}