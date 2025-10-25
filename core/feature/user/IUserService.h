#pragma once
#include "User.h"

class IUserService
{
public:
    [[nodiscard]] std::optional<User> getUserById(int id) const;
    void updateUser(const User& user) const;
    void createUser(const User& user) const;

    [[nodiscard]] bool accessToUser(const std::string& lastName, const std::string& firstName, const std::string& password) const;

    virtual ~IUserService() = default;

private:
    [[nodiscard]] virtual std::optional<User> doGetUserById(int id) const = 0;
    virtual void doUpdateUser(const User& user) const = 0;
    virtual void doCreateUser(const User& user) const = 0;

    [[nodiscard]] virtual bool doAccessToUser(const std::string& lastName, const std::string& firstName, const std::string& password) const = 0;

};

inline std::optional<User> IUserService::getUserById(const int id) const { return doGetUserById(id); }
inline void IUserService::updateUser(const User& user) const { doUpdateUser(user); }
inline void IUserService::createUser(const User& user) const { doCreateUser(user); }

inline bool IUserService::accessToUser(const std::string &lastName, const std::string &firstName, const std::string &password) const {return doAccessToUser(lastName, firstName, password); }
