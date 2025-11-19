#pragma once
#include <soci/soci.h>
#include "User.h"
#include "IUserDao.h"

class UserDao final : public IUserDao
{
public:
    explicit UserDao(soci::connection_pool& pool) : _pool(pool) {}
    UserDao(const UserDao&) = delete;
    UserDao& operator=(const UserDao&) = delete;
private:
    soci::connection_pool& _pool;

    std::optional<User> doGetById(int id) const override;
    std::optional<User> doGetByPhone(const std::string& phone) const override;
    void doUpdate(const User& user) const override;
    void doCreate(const User& user) const override;
};