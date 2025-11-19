#include "doctest.h"
#include "DBTestFixture.h"
#include "UserDao.h"
#include "UserService.h"
#include "Hasher.h"

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
        CHECK(retrieved.has_value());
        CHECK_EQ(retrieved->firstName, user.firstName);
        CHECK_EQ(retrieved->lastName, user.lastName);
        CHECK_EQ(retrieved->phone, user.phone);
        CHECK(Hasher::verifyPin(user.passwordHash, retrieved->passwordHash));
        CHECK_EQ(retrieved->status, user.status);
        CHECK_EQ(retrieved->failedLoginCount, user.failedLoginCount);
    }

    SUBCASE("getUserById should return user with given id if such user exists")
    {
        service.createUser(user);
        auto retrieved = service.getUserById(user.id);
        CHECK(retrieved.has_value());
        CHECK_EQ(retrieved->id, user.id);
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
        CHECK(retrieved.has_value());
        CHECK_EQ(retrieved->phone, user.phone);
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
        CHECK(retrieved.has_value());
        CHECK_EQ(retrieved->firstName, updated.firstName);
        CHECK_EQ(retrieved->lastName, updated.lastName);
        CHECK_EQ(retrieved->phone, updated.phone);
        CHECK(Hasher::verifyPin(updated.passwordHash, retrieved->passwordHash));
        CHECK_EQ(retrieved->status, updated.status);
        CHECK_EQ(retrieved->failedLoginCount, updated.failedLoginCount);
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