#include "ScheduledTransferDao.h"
#include "ScheduledTransferSql.h"

using namespace std;

ScheduledTransfer ScheduledTransferDao::doGetById(int id) const
{
    soci::session sql(_pool);
    ScheduledTransfer res;
    sql << scheduled_transfer_sql::getById,
        soci::use(id, "id"), soci::into(res);
    return res;
}

std::vector<ScheduledTransfer> ScheduledTransferDao::doGetAllActiveBeforeDate(const std::tm& date) const
{
    soci::session sql(_pool);
    vector<ScheduledTransfer> res;
    soci::rowset<ScheduledTransfer> rs = (sql.prepare
        << scheduled_transfer_sql::getActiveBeforeDate, soci::use(date, "date"));
    for (const ScheduledTransfer& st : rs)
        res.push_back(st);
    return res;
}

std::vector<ScheduledTransfer> ScheduledTransferDao::doGetByFromCardId(int id) const
{
    soci::session sql(_pool);
    vector<ScheduledTransfer> res;
    soci::rowset<ScheduledTransfer> rs = (sql.prepare
        << scheduled_transfer_sql::getByFromCardId, soci::use(id, "id"));
    for (const ScheduledTransfer& st : rs)
        res.push_back(st);
    return res;
}

void ScheduledTransferDao::doCreate(const ScheduledTransfer& transfer) const
{
    soci::session sql(_pool);
    sql << scheduled_transfer_sql::create,
        soci::use(transfer);
}

void ScheduledTransferDao::doUpdate(ScheduledTransfer transfer) const
{
    soci::session sql(_pool);
    sql << scheduled_transfer_sql::update,
        soci::use(transfer);
}

void ScheduledTransferDao::doDeleteById(int id) const
{
    soci::session sql(_pool);
    sql << scheduled_transfer_sql::deleteById, soci::use(id, "id");
}