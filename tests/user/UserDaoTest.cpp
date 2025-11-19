#include "doctest.h"
#include "DBTestFixture.h"
#include "UserDao.h"

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
        CHECK(retrieved.has_value());
        CHECK_EQ(retrieved->firstName, user.firstName);
        CHECK_EQ(retrieved->lastName, user.lastName);
        CHECK_EQ(retrieved->phone, user.phone);
        CHECK_EQ(retrieved->passwordHash, user.passwordHash);
        CHECK_EQ(retrieved->status, user.status);
        CHECK_EQ(retrieved->failedLoginCount, user.failedLoginCount);
    }

    SUBCASE("Get by id should return user with given id if such user exists")
    {
        dao.create(user);
        auto retrieved = dao.getById(user.id);
        CHECK(retrieved.has_value());
        CHECK_EQ(retrieved->id, user.id);
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
        CHECK(retrieved.has_value());
        CHECK_EQ(retrieved->phone, user.phone);
    }

    SUBCASE("Update should save modified data to database")
    {
        dao.create(user);
        auto retrieved = dao.getById(1);
        retrieved->firstName = "Changed Name";
        retrieved->lastName = "Changed Last Name";

        CHECK_NOTHROW(dao.update(*retrieved));
        auto updated = dao.getById(1);

        CHECK(updated.has_value());
        CHECK_EQ(updated->firstName, retrieved->firstName);
        CHECK_EQ(updated->lastName, retrieved->lastName);
    }

}