#include "UserService.h"

#include <iostream>

#include "Hasher.h"

UserService::UserService(IUserDao &dao) : _userDao(dao) {
}

std::optional<User> UserService::doGetUserById(int id) const {
    return _userDao.getById(id);
}

std::optional<User> UserService::doGetUserByPhone(const std::string &phone) const {
    User user = _userDao.getByPhone(phone);
    if (user.id == 0 || user.phone.empty()) { return std::nullopt; }
    return user;
}

void UserService::doUpdateUser(const User &user) const {
    User newUser = user;
    newUser.passwordHash = Hasher::hashPin(user.passwordHash);
    _userDao.update(newUser);
}

void UserService::doCreateUser(const User &user) const {
    User newUser = user;
    newUser.passwordHash = Hasher::hashPin(user.passwordHash);
    _userDao.create(newUser);
}

bool UserService::doAccessToUser(const std::string &phone, const std::string &password) const {
    User user = _userDao.getByPhone(phone);
    if (user.id == 0 || user.phone.empty()) { return false; }
    return Hasher::verifyPin(password, user.passwordHash);
}
