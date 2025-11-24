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

    inline const auto getByCurrencies =
        "SELECT id, base_currency, target_currency, rate "
        "FROM exchange_rate "
        "WHERE base_currency = :from AND target_currency = :to";
}