#pragma once

namespace scheduled_transfer_sql
{
    inline const auto getById =
        "SELECT * FROM scheduled_transfer WHERE id = :id";

    inline const auto getActiveBeforeDate =
    "SELECT * FROM scheduled_transfer WHERE next_tun < ? AND active = 1";

    inline const auto getByFromCardId =
        "SELECT * FROM scheduled_transfer WHERE from_card_id = ?";

    inline const auto create =
        "INSERT INTO scheduled_transfer ("
        "id, from_card_id, to_card_id, amount, currency_code, frequency, "
        "next_tun, active, description, comment) "
        "VALUES (:t)";

    inline const auto update =
        "UPDATE scheduled_transfer SET "
        "from_card_id = :t, "
        "to_card_id = :t, "
        "amount = :t, "
        "currency_code = :t, "
        "frequency = :t, "
        "next_tun = :t, "
        "active = :t, "
        "description = :t, "
        "comment = :t "
        "WHERE id = :t";

    inline const auto deleteById =
        "DELETE FROM scheduled_transfer WHERE id = :id";
}