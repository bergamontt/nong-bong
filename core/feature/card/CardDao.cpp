#include <string>
#include "CardDao.h"
#include "CardSql.h"
#include <iostream>

using namespace std;

std::optional<Card> CardDao::doGetById(int id) const
{
    soci::session sql(_pool);
    Card res;
    sql << card_sql::getById,
        soci::use(id, "id"), soci::into(res);
    return res;
}

std::optional<Card> CardDao::doGetByNumber(const string& number) const
{
    soci::session sql(_pool);
    soci::rowset<Card> rs = (sql.prepare << card_sql::getByNumber,
        soci::use(number, "number"));
    auto it = rs.begin();
    if (it == rs.end()) {
        return nullopt;
    }
    return *it;
}

std::vector<Card> CardDao::doGetByUserId(int id) const 
{
    soci::session sql(_pool);
    vector<Card> res;
    const soci::rowset<Card> rs = (sql.prepare
        << card_sql::getByUserId, soci::use(id));
    for (const Card& c : rs)
        res.push_back(c);
    return res;
}

std::vector<Card> CardDao::doGetByUserIdAndStatus(int id, const Card::Status status) const
{
    soci::session sql(_pool);
    vector<Card> res;
    std::string statusStr = statusToString(status);
    const soci::rowset<Card> rs = (sql.prepare
        << card_sql::getByUserIdAndStatus,
        soci::use(id),
        soci::use(statusStr));
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

void CardDao::doUpdatePin(const Card& card) const
{
    soci::session sql(_pool);
    sql << card_sql::update_pin,
        soci::use(card);
}

void CardDao::doCreate(const Card& card) const
{
    soci::session sql(_pool);
    std::string statusStr = statusToString(card.status);
    sql << card_sql::create,
        soci::use(card);
}