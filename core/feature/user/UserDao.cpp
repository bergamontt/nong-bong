#include "UserDao.h"
#include "UserSql.h"

using namespace std;

User UserDao::doGetById(int id) const
{
    soci::session sql(_pool);
    User res;
    sql << user_sql::getById,
        soci::use(id, "id"), soci::into(res);
    return res;
}

User UserDao::doGetByPhone(const std::string& phone) const
{
    soci::session sql(_pool);
    User res;
    sql << user_sql::getByPhone,
        soci::use(phone, "phone"), soci::into(res);
    return res;
}

void UserDao::doUpdate(const User& user) const
{
    soci::session sql(_pool);
    sql << user_sql::update,
        soci::use(user);
}

void UserDao::doCreate(const User& user) const
{
    soci::session sql(_pool);
    sql <<  user_sql::create,
        soci::use(user.firstName, "first_name"),
        soci::use(user.lastName, "last_name"),
        soci::use(user.email, "email"),
        soci::use(user.phone, "phone"),
        soci::use(user.createdAt, "created_at"),
        soci::use(user.passwordHash, "password_hash"),
        soci::use(user.status, "status"),
        soci::use(user.failedLoginCount, "failed_login_count"),
        soci::use(user.blockedUntil, "blocked_until");
}