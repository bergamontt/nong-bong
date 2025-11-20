#include "doctest.h"
#include "DBTestFixture.h"
#include "CurrencyDao.h"
#include "CurrencyService.h"

TEST_CASE_FIXTURE(DBTestFixture, "CurrencyService API Integration Test")
{
    CurrencyDao dao(pool());
    CurrencyService service(dao);

    Currency curr;
    curr.code = "Code";
    curr.name = "Name";
    curr.minorUnit = 1;

    SUBCASE("create should insert a valid currency")
    {
        CHECK_NOTHROW(service.create(curr));
        auto retrieved = service.getCurrencyByCode(curr.code);
        CHECK(retrieved.has_value());
        CHECK_EQ(retrieved->code, curr.code);
        CHECK_EQ(retrieved->name, curr.name);
        CHECK_EQ(retrieved->minorUnit, curr.minorUnit);
    }

    SUBCASE("getCurrencyByCode should return valid currency with given code if such currency exists")
    {
        service.create(curr);
        auto retrieved = service.getCurrencyByCode(curr.code);
        CHECK_EQ(retrieved.has_value(), true);
        CHECK_EQ(retrieved->code, curr.code);
        CHECK_EQ(retrieved->name, curr.name);
        CHECK_EQ(retrieved->minorUnit, curr.minorUnit);
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
        CHECK_EQ(retrieved.size(), 1);
        CHECK_EQ(retrieved[0].code, curr.code);
        CHECK_EQ(retrieved[0].name, curr.name);
        CHECK_EQ(retrieved[0].minorUnit, curr.minorUnit);
    }

}