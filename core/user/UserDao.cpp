#include "UserDao.h"

using namespace std;

User UserDao::doGetById(int id) const
{
    soci::session sql(_pool);
    User res;
    sql << "SELECT * FROM user WHERE id = :id",
        soci::use(id, "id"), soci::into(res);
    return res;
}

void UserDao::doUpdate(const User& user) const
{
    soci::session sql(_pool);
    sql << "UPDATE user SET "
        "first_name = :u, "
        "last_name = :u, "
        "email = :u, "
        "phone = :u, "
        "created_at = :u, "
        "password_hash = :u, "
        "status = :u, "
        "failed_login_count = :u, "
        "blocked_until = :u "
        "WHERE id = :u",
        soci::use(user);
}

void UserDao::doCreate(const User& user) const
{
    soci::session sql(_pool);
    sql << "INSERT INTO user ("
        "id, first_name, last_name, email, phone, created_at, "
        "password_hash, status, failed_login_count, blocked_until) "
        "VALUES (:u)",
        soci::use(user);
}