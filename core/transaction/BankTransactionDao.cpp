#include "BankTransactionDao.h"

using namespace std;

BankTransaction BankTransactionDao::doGetById(int id) const
{
    soci::session sql(_pool);
    BankTransaction res;
    sql << "SELECT * FROM bank_transaction WHERE id = :id",
        soci::use(id, "id"), soci::into(res);
    return res;
}

std::vector<BankTransaction> BankTransactionDao::doGetByFromCardId(int id) const
{
    soci::session sql(_pool);
    vector<BankTransaction> res;
    soci::rowset<BankTransaction> rs = (sql.prepare
        << "SELECT * FROM bank_transaction WHERE from_card_id = :id", soci::use(id, "id"));
    for (const BankTransaction& bt : rs)
        res.push_back(bt);
    return res;
}

void BankTransactionDao::doCreateById(const BankTransaction& transfer) const
{
    soci::session sql(_pool);
    sql << "INSERT INTO bank_transaction ("
        "id, created_at, type, from_card_id, to_card_id, amount, "
        "currency_code, description, comment, status) "
        "VALUES (:t)",
        soci::use(transfer);
}