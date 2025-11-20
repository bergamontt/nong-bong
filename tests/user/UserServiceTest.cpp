#include "doctest.h"
#include "DBTestFixture.h"
#include "UserDao.h"
#include "UserService.h"
#include "UserTestUtils.h"

TEST_CASE_FIXTURE(DBTestFixture, "UserService API Integration Test")
{
    UserDao dao(pool());
    const UserService service(dao);

    User user;
    user.id = 1;
    user.firstName = "First Name";
    user.lastName = "Last Name";
    user.phone = "12345";
    user.passwordHash = "password";
    user.status = "active";
    user.failedLoginCount = 0;

    SUBCASE("createUser should insert valid user")
    {
        CHECK_NOTHROW(service.createUser(user));
        auto retrieved = service.getUserById(user.id);
        REQUIRE(retrieved.has_value());
        assertUserEqualsHashed(user, *retrieved);
    }

    SUBCASE("getUserById should return user with given id if such user exists")
    {
        service.createUser(user);
        auto retrieved = service.getUserById(user.id);
        REQUIRE(retrieved.has_value());
        assertUserEqualsHashed(user, *retrieved);
    }

    SUBCASE("getUserById should return no value unless such user exists")
    {
        auto retrieved = service.getUserById(user.id);
        CHECK_FALSE(retrieved.has_value());
    }

    SUBCASE("getUserByPhone should return user with given phone if such user exists")
    {
        service.createUser(user);
        auto retrieved = service.getUserByPhone("12345");
        REQUIRE(retrieved.has_value());
        assertUserEqualsHashed(user, *retrieved);
    }

    SUBCASE("getUserByPhone should return no value unless such user exists")
    {
        auto retrieved = service.getUserByPhone("12345");
        CHECK_FALSE(retrieved.has_value());
    }

    SUBCASE("updateUser should modify existing user in database")
    {
        service.createUser(user);

        User updated = user;
        updated.firstName = "New First Name";
        updated.lastName = "New Last Name";
        updated.phone = "54321";
        user.passwordHash = "newPassword";

        CHECK_NOTHROW(service.updateUser(updated));

        auto retrieved = service.getUserById(user.id);
        REQUIRE(retrieved.has_value());
        assertUserEqualsHashed(updated, *retrieved);
    }

    SUBCASE("accessToUser should return true for valid phone and password")
    {
        service.createUser(user);
        CHECK(service.accessToUser(user.phone, user.passwordHash));
    }

    SUBCASE("accessToUser should return false for invalid password")
    {
        service.createUser(user);
        CHECK_FALSE(service.accessToUser(user.phone, "invalid"));
    }

    SUBCASE("accessToUser should return false for invalid phone")
    {
        service.createUser(user);
        CHECK_FALSE(service.accessToUser("invalid", user.passwordHash));
    }

}