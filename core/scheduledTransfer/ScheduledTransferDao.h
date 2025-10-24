#pragma once
#include <soci/soci.h>
#include "ScheduledTransfer.h"
#include "IScheduledTransferDao.h"

class ScheduledTransferDao
{
private:
    soci::connection_pool& _pool;

    ScheduledTransfer doGetById(int id) const;
    std::vector<ScheduledTransfer> doGetByFromCardId(int id) const;
    void doCreate(const ScheduledTransfer& transfer) const;
    void doUpdate(ScheduledTransfer transfer) const;
    void doDeleteById(int id) const;

public:
    ScheduledTransferDao(soci::connection_pool& pool) : _pool(pool) {}
    ScheduledTransferDao(const ScheduledTransferDao&) = delete;
    ScheduledTransferDao& operator=(const ScheduledTransferDao&) = delete;
};