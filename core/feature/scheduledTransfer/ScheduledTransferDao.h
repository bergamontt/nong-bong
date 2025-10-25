#pragma once
#include <soci/soci.h>
#include "ScheduledTransfer.h"
#include "IScheduledTransferDao.h"

class ScheduledTransferDao final : public IScheduledTransferDao
{
public:
    explicit ScheduledTransferDao(soci::connection_pool& pool) : _pool(pool) {}
    ScheduledTransferDao(const ScheduledTransferDao&) = delete;
    ScheduledTransferDao& operator=(const ScheduledTransferDao&) = delete;
private:
    soci::connection_pool& _pool;

    ScheduledTransfer doGetById(int id) const override;
    std::vector<ScheduledTransfer> doGetByFromCardId(int id) const override;
    void doCreate(const ScheduledTransfer& transfer) const override;
    void doUpdate(ScheduledTransfer transfer) const override;
    void doDeleteById(int id) const override;
};