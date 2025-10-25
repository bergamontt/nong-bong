#pragma once

namespace bank_transaction_sql
{
    inline const auto getById =
        "SELECT * FROM bank_transaction WHERE id = :id";

    inline const auto getByFromCardId =
        "SELECT * FROM bank_transaction WHERE from_card_id = :id";

    inline const auto create =
        "INSERT INTO bank_transaction ("
        "id, created_at, type, from_card_id, to_card_id, amount, "
        "currency_code, description, comment, status) "
        "VALUES (:t)";
}