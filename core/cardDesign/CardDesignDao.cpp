#include "CardDesignDao.h"

using namespace std;

CardDesign CardDesignDao::doGetById(int id) const
{
    soci::session sql(_pool);
    CardDesign res;
    sql << "SELECT * FROM card_design WHERE id = :id",
        soci::use(id, "id"), soci::into(res);
    return res;
}

std::vector<CardDesign> CardDesignDao::doGetAll() const
{
    soci::session sql(_pool);
    vector<CardDesign> res;
    soci::rowset<CardDesign> rs = (sql.prepare << "SELECT * FROM card_design");
    for (const CardDesign& cd : rs)
        res.push_back(cd);
    return res;
}
