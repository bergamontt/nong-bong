#include "doctest.h"
#include "DBTestFixture.h"
#include "UserDao.h"
#include "UserTestUtils.h"

TEST_CASE_FIXTURE(DBTestFixture, "UserDao API test")
{
    const UserDao dao(pool());

    User user;
    user.id = 1;
    user.firstName = "Name";
    user.lastName = "Last Name";
    user.phone = "phone";
    user.passwordHash = "password";
    user.status = "active";
    user.failedLoginCount = 0;

    SUBCASE("Create should insert valid user")
    {
        CHECK_NOTHROW(dao.create(user));
        auto retrieved = dao.getById(1);
        REQUIRE(retrieved.has_value());
        assertUserEquals(user, *retrieved);
    }

    SUBCASE("Get by id should return user with given id if such user exists")
    {
        dao.create(user);
        auto retrieved = dao.getById(user.id);
        REQUIRE(retrieved.has_value());
        assertUserEquals(user, *retrieved);
    }

    SUBCASE("Get by id should return no value unless such user exists")
    {
        auto retrieved = dao.getById(user.id);
        CHECK_FALSE(retrieved.has_value());
    }

    SUBCASE("Get by phone should return user with the given phone")
    {
        dao.create(user);
        auto retrieved = dao.getById(user.id);
        REQUIRE(retrieved.has_value());
        assertUserEquals(user, *retrieved);
    }

    SUBCASE("Update should save modified data to database")
    {
        dao.create(user);
        auto retrieved = dao.getById(1);
        retrieved->firstName = "Changed Name";
        retrieved->lastName = "Changed Last Name";

        CHECK_NOTHROW(dao.update(*retrieved));
        auto updated = dao.getById(1);
        REQUIRE(updated.has_value());
        assertUserEquals(*updated, *retrieved);
    }

}