#pragma once
#include "IUserDao.h"
#include "IUserService.h"
#include "User.h"

class UserService : public IUserService
{
    std::shared_ptr<IUserDao> dao;

    explicit UserService(std::shared_ptr<IUserDao> dao);

    [[nodiscard]] std::optional<User> doGetUserById(int id) const override;
    void doUpdateUser(const User& user) const override;
    void doCreateUser(const User& user) const override;

    [[nodiscard]] bool doAccessToUser(const std::string& lastName, const std::string& firstName, const std::string& password) const override;

};