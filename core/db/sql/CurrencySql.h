#pragma once

namespace currency_sql
{
    inline const auto getByCode =
        "SELECT * FROM currency WHERE code = :code";

    inline const auto getAll =
        "SELECT * FROM currency";
}