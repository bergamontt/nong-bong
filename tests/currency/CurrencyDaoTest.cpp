#include "doctest.h"
#include "DBTestFixture.h"
#include "CurrencyDao.h"
#include "EntityTestUtils.h"

TEST_CASE_FIXTURE(DBTestFixture, "CurrencyDao API test")
{
    const CurrencyDao dao(pool());

    Currency usd;
    usd.code = "USD";
    usd.name = "US Dollar";
    usd.minorUnit = 100;

    Currency eur;
    eur.code = "EUR";
    eur.name = "Euro";
    eur.minorUnit = 100;

    SUBCASE("Create should insert valid currency")
    {
        CHECK_NOTHROW(dao.create(usd));
        auto retrieved = dao.getByCode("USD");
        REQUIRE(retrieved.has_value());
        assertCurrencyEquals(*retrieved, usd);
    }

    SUBCASE("Get by code should return currency with the given code if such currency exists")
    {
        dao.create(eur);
        auto retrieved = dao.getByCode("EUR");
        REQUIRE(retrieved.has_value());
        assertCurrencyEquals(*retrieved, eur);
    }

    SUBCASE("Get by code should return no value unless such currency exists")
    {
        auto retrieved = dao.getByCode("EUR");
        CHECK_FALSE(retrieved.has_value());
    }

    SUBCASE("Get all should return all currencies")
    {
        dao.create(usd);
        dao.create(eur);
        auto retrieved = dao.getAll();
        REQUIRE_EQ(retrieved.size(), 2);
        assertCurrencyEquals(retrieved[0], usd);
        assertCurrencyEquals(retrieved[1], eur);
    }

}