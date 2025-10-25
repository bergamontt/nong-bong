#include "CardDao.h"
#include "CardSql.h"

using namespace std;

Card CardDao::doGetById(int id) const 
{
    soci::session sql(_pool);
    Card res;
    sql << card_sql::getById,
        soci::use(id, "id"), soci::into(res);
    if (!sql.got_data())
        throw runtime_error("Card not found: " + id);
    return res;
}

std::vector<Card> CardDao::doGetByUserId(int id) const 
{
    soci::session sql(_pool);
    vector<Card> res;
    const soci::rowset<Card> rs = (sql.prepare
        << card_sql::getByUserId, soci::use(id, "id"));
    for (const Card& c : rs)
        res.push_back(c);
    return res;
}

void CardDao::doUpdate(const Card& card) const
{
    soci::session sql(_pool);
    sql << card_sql::update,
        soci::use(card);
}

void CardDao::doCreate(const Card& card) const
{
    soci::session sql(_pool);
    sql << card_sql::create,
        soci::use(card);
}