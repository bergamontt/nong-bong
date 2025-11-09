#pragma once

namespace user_sql {
    inline const auto getById =
            "SELECT * FROM user WHERE id = :id";

    inline const auto getByPhone =
            "SELECT * FROM user WHERE phone = :phone";

    inline const auto update =
            "UPDATE user SET "
            "first_name = :u, "
            "last_name = :u, "
            "email = :u, "
            "phone = :u, "
            "created_at = :u, "
            "password_hash = :u, "
            "status = :u, "
            "failed_login_count = :u, "
            "blocked_until = :u "
            "WHERE id = :u";

    inline const auto create =
            "INSERT INTO user ("
            "first_name, last_name, email, phone, created_at, "
            "password_hash, status, failed_login_count, blocked_until) "
            "VALUES (:first_name, :last_name, :email, :phone, :created_at, "
            ":password_hash, :status, :failed_login_count, :blocked_until)";
}
