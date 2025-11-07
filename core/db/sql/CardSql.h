#pragma once

namespace card_sql {
    inline const auto getById =
            "SELECT * FROM card WHERE id = :id";

    inline const auto getByUserId =
            "SELECT * FROM card WHERE user_id = ?";

    inline const auto getByUserIdAndStatus =
            "SELECT * FROM card WHERE user_id = ? AND status = ?";

    inline const auto update =
            "UPDATE card SET "
            "user_id = :user_id, "
            "card_number = :card_number, "
            "allow_credit = :allow_credit, "
            "credit_limit = :credit_limit, "
            "currency_code = :currency_code, "
            "balance = :balance, "
            "daily_limit = :daily_limit, "
            "design_id = :design_id, "
            "status = :status, "
            "failed_access_count = :failed_access_count, "
            "blocked_until = :blocked_until "
            "WHERE id = :id";

    inline const auto update_pin =
            "UPDATE card SET "
            "pin_hash = :pin_hash "
            "WHERE id = :id";

    inline const auto create =
            "INSERT INTO card (id, user_id, card_number, allow_credit, "
            "credit_limit, currency_code, balance, daily_limit, design_id, "
            "pin_hash, status, failed_access_count, blocked_until) "
            "VALUES (:c)";
}
