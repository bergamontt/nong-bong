#pragma once
#include "IUserDao.h"
#include "IUserService.h"
#include "User.h"

class UserService final : public IUserService
{
public:
    explicit UserService(IUserDao& dao);
    UserService(const UserService&) = delete;
    UserService& operator=(const UserService&)& = delete;

private:
    IUserDao& _userDao;

    std::optional<User> doGetUserById(int id) const override;
    std::optional<User> doGetUserByPhone(const std::string &phone) const override;
    void doUpdateUser(const User& user) const override;
    void doCreateUser(const User& user) const override;
    bool doAccessToUser(const std::string& phone, const std::string& password) const override;

};