#include "CardDesignDao.h"
#include "CardDesignSql.h"

using namespace std;

CardDesign CardDesignDao::doGetById(int id) const
{
    soci::session sql(_pool);
    CardDesign res;
    sql << card_design_sql::getById,
        soci::use(id, "id"), soci::into(res);
    return res;
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
