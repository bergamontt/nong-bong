#include <string>
#include "CardDao.h"
#include "CardSql.h"

using namespace std;

std::optional<Card> CardDao::doGetById(int id) const
{
    soci::session sql(_pool);
    Card res;
    sql << card_sql::getById,
        soci::use(id, "id"), soci::into(res);
    return sql.got_data() ? std::optional{res} : std::nullopt;
}

std::optional<Card> CardDao::doGetByNumber(const string& number) const
{
    soci::session sql(_pool);
    Card res;
    sql << card_sql::getByNumber,
        soci::use(number, "number"), soci::into(res);
    return sql.got_data() ? std::optional{res} : std::nullopt;
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

int CardDao::doGetSpendingsSince(int cardId, std::tm time) const
{
    soci::session sql(_pool);
    int total = 0;
    sql << card_sql::getSpendingsSince,
        soci::use(cardId, "cardId"),
        soci::use(time, "since"),
        soci::into(total);
    return total;
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