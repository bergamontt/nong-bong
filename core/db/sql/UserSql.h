#pragma once

namespace user_sql {
    inline const auto getById =
            "SELECT * FROM user WHERE id = :id";

    inline const auto getByPhone =
            "SELECT * FROM user WHERE phone = :phone";

    inline const auto update =
            "UPDATE user SET "
            "first_name = :first_name, "
            "last_name = :last_name, "
            "email = :email, "
            "phone = :phone, "
            "created_at = :created_at, "
            "password_hash = :password_hash, "
            "status = :status, "
            "failed_login_count = :failed_login_count, "
            "blocked_until = :blocked_until "
            "WHERE id = :id";

    inline const auto create =
            "INSERT INTO user ("
            "first_name, last_name, email, phone, created_at, "
            "password_hash, status, failed_login_count, blocked_until) "
            "VALUES (:first_name, :last_name, :email, :phone, :created_at, "
            ":password_hash, :status, :failed_login_count, :blocked_until)";
}
