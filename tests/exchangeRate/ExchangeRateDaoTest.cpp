#include "doctest.h"
#include "DBTestFixture.h"
#include "ExchangeRateDao.h"

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
        CHECK(retrieved.has_value());
        CHECK_EQ(retrieved->baseCurrency, "USD");
        CHECK_EQ(retrieved->targetCurrency, "EUR");
        REQUIRE_EQ(retrieved->rate, doctest::Approx(0.93));
    }

    SUBCASE("getById should return valid exchange rate with given id if such rate exists")
    {
        dao.create(rate1);
        auto retrieved = dao.getById(rate1.id);
        CHECK(retrieved.has_value());
        CHECK_EQ(retrieved->baseCurrency, rate1.baseCurrency);
        CHECK_EQ(retrieved->targetCurrency, rate1.targetCurrency);
        CHECK_EQ(retrieved->rate, rate1.rate);
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
        auto all = dao.getAll();
        CHECK_EQ(all.size(), 2);
        CHECK_EQ(all[0].baseCurrency, "USD");
        CHECK_EQ(all[1].baseCurrency, "EUR");
    }

}