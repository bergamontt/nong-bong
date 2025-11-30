#include "doctest.h"
#include "DBTestFixture.h"
#include "CurrencyDao.h"
#include "ExchangeRateDao.h"
#include "CurrencyService.h"
#include "EntityTestUtils.h"

TEST_CASE_FIXTURE(DBTestFixture, "CurrencyService API Integration Test")
{
    CurrencyDao curDao(pool());
    ExchangeRateDao excDao(pool());
    CurrencyService service(curDao, excDao);

    Currency curr;
    curr.code = "Code";
    curr.name = "Name";
    curr.minorUnit = 1;

    SUBCASE("create should insert a valid currency")
    {
        CHECK_NOTHROW(service.create(curr));
        auto retrieved = service.getCurrencyByCode(curr.code);
        REQUIRE(retrieved.has_value());
        assertCurrencyEquals(*retrieved, curr);
    }

    SUBCASE("getCurrencyByCode should return valid currency with given code if such currency exists")
    {
        service.create(curr);
        auto retrieved = service.getCurrencyByCode(curr.code);
        REQUIRE(retrieved.has_value());
        assertCurrencyEquals(*retrieved, curr);
    }

    SUBCASE("getCurrencyByCode should return no value unless such currency exists")
    {
        auto retrieved = service.getCurrencyByCode(curr.code);
        CHECK_FALSE(retrieved.has_value());
    }

    SUBCASE("getAllCurrencies should return all inserted currencies")
    {
        service.create(curr);
        auto retrieved = service.getAllCurrencies();
        REQUIRE_EQ(retrieved.size(), 1);
        assertCurrencyEquals(retrieved[0], curr);
    }

}