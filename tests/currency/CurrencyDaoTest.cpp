#include "doctest.h"
#include "DBTestFixture.h"
#include "CurrencyDao.h"

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
        Currency retrieved = dao.getByCode("USD");
        CHECK_EQ(retrieved.code, usd.code);
        CHECK_EQ(retrieved.name, usd.name);
        CHECK_EQ(retrieved.minorUnit, usd.minorUnit);
    }

    SUBCASE("Get by code should return currency with the given code")
    {
        dao.create(eur);
        Currency retrieved = dao.getByCode("EUR");
        CHECK_EQ(retrieved.code, eur.code);
        CHECK_EQ(retrieved.name, eur.name);
        CHECK_EQ(retrieved.minorUnit, eur.minorUnit);
    }

    SUBCASE("Get all should return all currencies")
    {
        dao.create(usd);
        dao.create(eur);
        auto all = dao.getAll();
        CHECK_EQ(all.size(), 2);
        CHECK_EQ(all[0].code, "USD");
        CHECK_EQ(all[1].code, "EUR");
    }

}