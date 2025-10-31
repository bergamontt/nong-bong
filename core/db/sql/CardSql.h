#pragma once

namespace card_sql
{
    inline const auto getById =
        "SELECT * FROM card WHERE id = :id";

    inline const auto getByUserId =
        "SELECT * FROM card WHERE user_id = ?";

    inline const auto getByUserIdAndStatus =
        "SELECT * FROM card WHERE user_id = ? AND status = ?";

    inline const auto update =
        "UPDATE card SET "
        "user_id = :c, "
        "card_number = :c, "
        "allow_credit = :c, "
        "credit_limit = :c, "
        "currency_code = :c, "
        "balance = :c, "
        "daily_limit = :c, "
        "design_id = :c, "
        "pin_hash = :c, "
        "status = :c, "
        "failed_access_count = :c, "
        "blocked_until = :c "
        "WHERE id = :c";

    inline const auto create =
        "INSERT INTO card (id, user_id, card_number, allow_credit, "
        "credit_limit, currency_code, balance, daily_limit, design_id, "
        "pin_hash, status, failed_access_count, blocked_until) "
        "VALUES (:c)";
}