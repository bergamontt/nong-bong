#pragma once

namespace exchange_rate_sql
{
    inline const auto getById =
        "SELECT * FROM exchange_rate WHERE id = :id";

    inline const auto getAll =
        "SELECT * FROM exchange_rate";

    inline const auto create =
        "INSERT INTO exchange_rate (base_currency, target_currency, rate) "
        "VALUES (:base_currency, :target_currency, :rate)";
}