#include "doctest.h"
#include "DBTestFixture.h"
#include "ExchangeRateDao.h"

TEST_CASE_FIXTURE(DBTestFixture, "ExchangeRateDao API test")
{
    const ExchangeRateDao dao(pool());

    ExchangeRate rate1;
    rate1.baseCurrency = "USD";
    rate1.targetCurrency = "EUR";
    rate1.rate = 0.93;

    ExchangeRate rate2;
    rate2.baseCurrency = "EUR";
    rate2.targetCurrency = "USD";
    rate2.rate = 1.08;

    SUBCASE("Create should insert valid exchange rate")
    {
        CHECK_NOTHROW(dao.create(rate1));
        ExchangeRate retrieved = dao.getById(1);
        CHECK_EQ(retrieved.baseCurrency, "USD");
        CHECK_EQ(retrieved.targetCurrency, "EUR");
        REQUIRE_EQ(retrieved.rate, doctest::Approx(0.93));
    }

    SUBCASE("GetById should return correct exchange rate")
    {
        dao.create(rate1);
        ExchangeRate retrieved = dao.getById(1);
        CHECK_EQ(retrieved.baseCurrency, rate1.baseCurrency);
        CHECK_EQ(retrieved.targetCurrency, rate1.targetCurrency);
        CHECK_EQ(retrieved.rate, rate1.rate);
    }

    SUBCASE("GetAll should return all exchange rates")
    {
        dao.create(rate1);
        dao.create(rate2);
        auto all = dao.getAll();
        CHECK_EQ(all.size(), 2);
        CHECK_EQ(all[0].baseCurrency, "USD");
        CHECK_EQ(all[1].baseCurrency, "EUR");
    }

}