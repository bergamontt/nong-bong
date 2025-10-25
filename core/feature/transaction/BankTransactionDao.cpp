#include "BankTransactionDao.h"
#include "BankTransactionSql.h"

using namespace std;

BankTransaction BankTransactionDao::doGetById(int id) const
{
    soci::session sql(_pool);
    BankTransaction res;
    sql << bank_transaction_sql::getById,
        soci::use(id, "id"), soci::into(res);
    return res;
}

std::vector<BankTransaction> BankTransactionDao::doGetByFromCardId(int id) const
{
    soci::session sql(_pool);
    vector<BankTransaction> res;
    soci::rowset<BankTransaction> rs = (sql.prepare
        << bank_transaction_sql::getByFromCardId, soci::use(id, "id"));
    for (const BankTransaction& bt : rs)
        res.push_back(bt);
    return res;
}

void BankTransactionDao::doCreate(const BankTransaction& transfer) const
{
    soci::session sql(_pool);
    sql << bank_transaction_sql::create,
        soci::use(transfer);
}