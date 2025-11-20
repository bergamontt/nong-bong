#include "doctest.h"
#include "DBTestFixture.h"
#include "ExchangeRateDao.h"
#include "ExchangeRateService.h"
#include "ExchangeRateTestUtils.h"

TEST_CASE_FIXTURE(DBTestFixture, "ExchangeRateService API Integration Test")
{
    ExchangeRateDao dao(pool());
    const ExchangeRateService service(dao);

    ExchangeRate rate;
    rate.id = 1;
    rate.baseCurrency = "USD";
    rate.targetCurrency = "EUR";
    rate.rate = 0.93;

    SUBCASE("create should insert valid exchange rate into database")
    {
        CHECK_NOTHROW(service.create(rate));
        auto retrieved = service.getExchangeRateById(rate.id);
        REQUIRE(retrieved.has_value());
        assertExchangeRateEquals(rate, *retrieved);
    }

    SUBCASE("getExchangeRateById should return exchange rate with the given id if such rate exists")
    {
        service.create(rate);
        auto retrieved = service.getExchangeRateById(rate.id);
        REQUIRE(retrieved.has_value());
        assertExchangeRateEquals(rate, *retrieved);
    }

    SUBCASE("getExchangeRateById should return no value unless record exists")
    {
        auto retrieved = service.getExchangeRateById(rate.id);
        CHECK_FALSE(retrieved.has_value());
    }

    SUBCASE("getAllExchangeRates should return all created exchange rates")
    {
        service.create(rate);
        auto retrieved = service.getAllExchangeRates();
        REQUIRE_EQ(retrieved.size(), 1);
        assertExchangeRateEquals(rate, retrieved[0]);
    }

}