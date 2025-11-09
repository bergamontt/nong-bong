#pragma once

namespace currency_sql
{
    inline const auto create =
        "INSERT INTO currency (code, name, minor_unit) "
        "VALUES (:code, :name, :minor_unit)";

    inline const auto getByCode =
        "SELECT * FROM currency WHERE code = :code";

    inline const auto getAll =
        "SELECT * FROM currency";
}