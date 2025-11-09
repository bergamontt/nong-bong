#pragma once
#include <optional>
#include <string>
#include <soci/soci.h>
#include <ctime>

class User
{
public:
    int id;
    std::string firstName;
    std::string lastName;
    std::optional<std::string> email;
    std::string phone;
    std::optional<std::tm> createdAt;
    std::string passwordHash;
    std::string status;
    int failedLoginCount;
    std::optional<std::tm> blockedUntil;
};

template <>
struct soci::type_conversion<User>
{
    typedef values base_type;
    static void from_base(values const& v, indicator, User& u)
    {
        u.id = v.get<int>("id");
        u.firstName = v.get<std::string>("first_name");
        u.lastName = v.get<std::string>("last_name");
        if (v.get_indicator("email") != soci::i_null)
            u.email = v.get<std::string>("email");
        else
            u.email.reset();

        u.phone = v.get<std::string>("phone");
        if (v.get_indicator("created_at") != soci::i_null)
            u.createdAt = v.get<std::tm>("created_at");
        else
            u.createdAt.reset();        u.passwordHash = v.get<std::string>("password_hash");
        u.status = v.get<std::string>("status");
        u.failedLoginCount = v.get<int>("failed_login_count");
        if (v.get_indicator("blocked_until") != soci::i_null)
            u.blockedUntil = v.get<std::tm>("blocked_until");
        else
            u.blockedUntil.reset();    }

    static void to_base(const User& u, values& v, indicator& ind)
    {
        v.set("id", u.id);
        v.set("first_name", u.firstName);
        v.set("last_name", u.lastName);
        v.set("email", u.email, u.email.has_value() ? i_ok : i_null);
        v.set("phone", u.phone);
        v.set("created_at", u.createdAt, u.createdAt.has_value() ? i_ok : i_null);
        v.set("password_hash", u.passwordHash);
        v.set("status", u.status);
        v.set("failed_login_count", u.failedLoginCount);
        v.set("blocked_until", u.blockedUntil, u.blockedUntil.has_value() ? i_ok : i_null);
    }
};