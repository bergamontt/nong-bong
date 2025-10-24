#include "CardDao.h"

using namespace std;

Card CardDao::doGetById(int id) const 
{
    soci::session sql(_pool);
    Card res;
    sql << "SELECT * FROM card WHERE id = :id",
        soci::use(id, "id"), soci::into(res);
    if (!sql.got_data())
        throw runtime_error("Card not found: " + id);
    return res;
}

std::vector<Card> CardDao::doGetByUserId(int id) const 
{
    soci::session sql(_pool);
    vector<Card> res;
    soci::rowset<Card> rs = (sql.prepare 
        << "SELECT * FROM card WHERE user_id = :id", soci::use(id, "id"));
    for (const Card& c : rs)
        res.push_back(c);
    return res;
}

void CardDao::doUpdate(const Card& card) const
{
    soci::session sql(_pool);
    sql << "UPDATE card SET "
        "user_id = :c, "
        "card_number = :c, "
        "allow_credit = :c, "
        "credit_limit = :c, "
        "currency_code = :c, "
        "balance = :c, "
        "daily_limit = :c, "
        "design_id = :c, "
        "pin_hash = :c, "
        "status = :c, "
        "failed_access_count = :c, "
        "blocked_until = :c "
        "WHERE id = :c",
        soci::use(card);
}


void CardDao::doCreate(const Card& card) const
{
    soci::session sql(_pool);
    sql << "INSERT INTO card (id, user_id, card_number, allow_credit, "
        "credit_limit, currency_code, balance, daily_limit, design_id, "
        "pin_hash, status, failed_access_count, blocked_until) "
        "VALUES (:c)",
        soci::use(card);
}