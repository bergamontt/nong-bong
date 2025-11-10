#pragma once

namespace scheduled_transfer_sql
{
    inline const auto getById =
        "SELECT * FROM scheduled_transfer WHERE id = :id";

    inline const auto getActiveBeforeDate =
        "SELECT * FROM scheduled_transfer WHERE next_run <= ? AND active = 1";

    inline const auto getByFromCardId =
        "SELECT * FROM scheduled_transfer WHERE from_card_id = ?";

    inline const auto create =
        "INSERT INTO scheduled_transfer ("
        "from_card_id, to_card_id, amount, currency_code, frequency, "
        "next_run, active, description, comment) "
        "VALUES (:from_card_id, :to_card_id, :amount, :currency_code, :frequency, "
        ":next_run, :active, :description, :comment)";

    inline const auto update =
        "UPDATE scheduled_transfer SET "
        "from_card_id = :from_card_id, "
        "to_card_id = :to_card_id, "
        "amount = :amount, "
        "currency_code = :currency_code, "
        "frequency = :frequency, "
        "next_run = :next_run, "
        "active = :active, "
        "description = :description, "
        "comment = :comment "
        "WHERE id = :id";

    inline const auto deleteById =
        "DELETE FROM scheduled_transfer WHERE id = :id";
}