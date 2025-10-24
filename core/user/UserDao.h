#pragma once
#include <soci/soci.h>
#include "User.h"
#include "IUserDao.h"

class UserDao
{
private:
    soci::connection_pool& _pool;

    User doGetById(int id) const;
    void doUpdate(const User& user) const;
    void doCreate(const User& user) const;

public:
    UserDao(soci::connection_pool& pool) : _pool(pool) {}
    UserDao(const UserDao&) = delete;
    UserDao& operator=(const UserDao&) = delete;
};