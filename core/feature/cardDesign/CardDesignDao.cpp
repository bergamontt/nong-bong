#include "CardDesignDao.h"
#include "CardDesignSql.h"

using namespace std;

std::optional<CardDesign> CardDesignDao::doGetById(int id) const
{
    soci::session sql(_pool);
    CardDesign res;
    sql << card_design_sql::getById,
        soci::use(id, "id"), soci::into(res);
    return sql.got_data() ? std::optional{res} : std::nullopt;
}

std::vector<CardDesign> CardDesignDao::doGetAll() const
{
    soci::session sql(_pool);
    vector<CardDesign> res;
    soci::rowset<CardDesign> rs = (sql.prepare << card_design_sql::getAll);
    for (const CardDesign& cd : rs)
        res.push_back(cd);
    return res;
}


void CardDesignDao::doCreate(const CardDesign& cardDesign) const {
    soci::session sql(_pool);
    sql << card_design_sql::create,
        soci::use(cardDesign);
}

void CardDesignDao::doDeleteAll() const {
    soci::session sql(_pool);
    sql << card_design_sql::deleteAll;
}
