#include "ScheduledTransferDao.h"

using namespace std;

ScheduledTransfer ScheduledTransferDao::doGetById(int id) const
{
    soci::session sql(_pool);
    ScheduledTransfer res;
    sql << "SELECT * FROM scheduled_transfer WHERE id = :id",
        soci::use(id, "id"), soci::into(res);
    return res;
}

std::vector<ScheduledTransfer> ScheduledTransferDao::doGetByFromCardId(int id) const
{
    soci::session sql(_pool);
    vector<ScheduledTransfer> res;
    soci::rowset<ScheduledTransfer> rs = (sql.prepare
        << "SELECT * FROM scheduled_transfer WHERE from_card_id = :id", soci::use(id, "id"));
    for (const ScheduledTransfer& st : rs)
        res.push_back(st);
    return res;
}

void ScheduledTransferDao::doCreate(const ScheduledTransfer& transfer) const
{
    soci::session sql(_pool);
    sql << "INSERT INTO scheduled_transfer ("
        "id, from_card_id, to_card_id, amount, currency_code, frequency, "
        "next_tun, active, description, comment) "
        "VALUES (:t)",
        soci::use(transfer);
}

void ScheduledTransferDao::doUpdate(ScheduledTransfer transfer) const
{
    soci::session sql(_pool);
    sql << "UPDATE scheduled_transfer SET "
        "from_card_id = :t, "
        "to_card_id = :t, "
        "amount = :t, "
        "currency_code = :t, "
        "frequency = :t, "
        "next_tun = :t, "
        "active = :t, "
        "description = :t, "
        "comment = :t "
        "WHERE id = :t",
        soci::use(transfer);
}

void ScheduledTransferDao::doDeleteById(int id) const
{
    soci::session sql(_pool);
    sql << "DELETE FROM scheduled_transfer WHERE id = :id", soci::use(id, "id");
}