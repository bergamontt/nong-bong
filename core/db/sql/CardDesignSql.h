#pragma once

namespace card_design_sql
{
    inline const auto getById =
        "SELECT * FROM card_design WHERE id = :id";

    inline const auto getAll =
        "SELECT * FROM card_design";

    inline const auto create =
        "INSERT INTO card_design (name, author, image_ref) "
        "VALUES (:name, :author, :image_ref)";

    inline const auto deleteAll =
        "DELETE FROM card_design";
}