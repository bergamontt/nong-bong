#include "UserService.h"
#include "Hasher.h"

UserService::UserService(IUserDao &dao):
    _userDao(dao)
{}

std::optional<User> UserService::doGetUserById(const int id) const
{
    return _userDao.getById(id);
}

std::optional<User> UserService::doGetUserByPhone(const std::string &phone) const
{
    return _userDao.getByPhone(phone);
}

void UserService::doUpdateUser(const User& user) const
{
    User newUser = user;
    newUser.passwordHash = Hasher::hashPin(user.passwordHash);
    _userDao.update(newUser);
}

void UserService::doCreateUser(const User& user) const
{
    User newUser = user;
    newUser.passwordHash = Hasher::hashPin(user.passwordHash);
    _userDao.create(newUser);
}

bool UserService::doAccessToUser(const std::string& phone, const std::string& password) const
{
    const std::optional<User> user = getUserByPhone(phone);
    if (!user.has_value())
        return false;
    return Hasher::verifyPin(password, user->passwordHash);
}