#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "DBTestFixture.h"
#include "UserDao.h"

TEST_CASE_FIXTURE(DBTestFixture, "UserDao should create valid user")
{
    UserDao dao(pool());

    User user;
    user.firstName = "Bob";
    user.lastName = "White";
    user.phone = "123";
    user.passwordHash = "password";
    user.status = "active";
    user.failedLoginCount = 0;

    dao.create(user);

    User retrieved = dao.getById(1);
    REQUIRE(retrieved.firstName == "Bob");
    REQUIRE(retrieved.lastName == "White");
    REQUIRE(retrieved.phone == "123");
    REQUIRE(retrieved.passwordHash == "password");
    REQUIRE(retrieved.status == "active");
    REQUIRE(retrieved.failedLoginCount == 0);
}