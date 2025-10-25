#pragma once
#include <vector>
#include "ScheduledTransfer.h"

class IScheduledTransferDao
{
public:
    ScheduledTransfer getById(int id) const;
    std::vector<ScheduledTransfer> getByFromCardId(int id) const;
    void create(const ScheduledTransfer& transfer) const;
    void update(const ScheduledTransfer& transfer) const;
    void deleteById(int id) const;
    virtual ~IScheduledTransferDao() = default;
private:
    virtual ScheduledTransfer doGetById(int id) const = 0;
    virtual std::vector<ScheduledTransfer> doGetByFromCardId(int id) const = 0;
    virtual void doUpdate(ScheduledTransfer transfer) const = 0;
    virtual void doDeleteById(int id) const = 0;
    virtual void doCreate(const ScheduledTransfer& transfer) const = 0;
};

inline ScheduledTransfer IScheduledTransferDao::getById(const int id) const
{
    return doGetById(id);
}

inline std::vector<ScheduledTransfer> IScheduledTransferDao::getByFromCardId(const int id) const
{
    return doGetByFromCardId(id);
}

inline void IScheduledTransferDao::create(const ScheduledTransfer& transfer) const
{
    doCreate(transfer);
}

inline void IScheduledTransferDao::update(const ScheduledTransfer& transfer) const
{
    doUpdate(transfer);
}

inline void IScheduledTransferDao::deleteById(const int id) const
{
    doDeleteById(id);
}
