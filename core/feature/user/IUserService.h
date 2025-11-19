#pragma once
#include "User.h"

class IUserService
{
public:
    std::optional<User> getUserById(int id) const;
    std::optional<User> getUserByPhone(const std::string &phone) const;
    void updateUser(const User& user) const;
    void createUser(const User& user) const;
    bool accessToUser(const std::string& phone, const std::string& password) const;
    virtual ~IUserService() = default;
private:
    virtual std::optional<User> doGetUserById(int id) const = 0;
    virtual std::optional<User> doGetUserByPhone(const std::string &phone) const = 0;
    virtual void doUpdateUser(const User& user) const = 0;
    virtual void doCreateUser(const User& user) const = 0;
    virtual bool doAccessToUser(const std::string& phone, const std::string& password) const = 0;
};

inline std::optional<User> IUserService::getUserById(const int id) const
{
    return doGetUserById(id);
}

inline std::optional<User> IUserService::getUserByPhone(const std::string &phone) const
{
    return doGetUserByPhone(phone);
}

inline void IUserService::updateUser(const User& user) const
{
    doUpdateUser(user);
}
inline void IUserService::createUser(const User& user) const
{
    doCreateUser(user);
}

inline bool IUserService::accessToUser(const std::string &phone, const std::string &password) const
{
    return doAccessToUser(phone, password);
}
