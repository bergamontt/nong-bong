#pragma once

namespace card_design_sql
{
    inline const auto getById =
        "SELECT * FROM card_design WHERE id = :id";
    inline const auto getAll =
        "SELECT * FROM card_design";
}