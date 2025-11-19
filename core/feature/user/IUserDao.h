#pragma once
#include "User.h"

class IUserDao
{
public:
    std::optional<User> getById(int id) const;
    std::optional<User> getByPhone(const std::string& phone) const;
    void update(const User& user) const;
    void create(const User& user) const;
protected:
    ~IUserDao() = default;
private:
    virtual std::optional<User> doGetById(int id) const = 0;
    virtual std::optional<User> doGetByPhone(const std::string& phone) const = 0;
    virtual void doUpdate(const User& user) const = 0;
    virtual void doCreate(const User& user) const = 0;
};

inline std::optional<User> IUserDao::getById(const int id) const
{
    return doGetById(id);
}

inline std::optional<User> IUserDao::getByPhone(const std::string& phone) const
{
    return doGetByPhone(phone);
}

inline void IUserDao::update(const User& user) const
{
    doUpdate(user);
}

inline void IUserDao::create(const User& user) const
{
    doCreate(user);
}
