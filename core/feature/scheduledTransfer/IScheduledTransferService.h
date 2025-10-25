#pragma once
#include <vector>
#include "ScheduledTransfer.h"

class IScheduledTransferService
{
public:
    [[nodiscard]] std::optional<ScheduledTransfer> getScheduledTransferById(int id) const;
    [[nodiscard]] std::vector<ScheduledTransfer> getAllScheduledTransfersFromCardId(int id) const;

    void createScheduledTransfer(const ScheduledTransfer& transfer) const;
    void updateScheduledTransfer(const ScheduledTransfer& transfer) const;
    void deleteScheduledTransfer(int id) const;

    void executeAllScheduledTransfersByNow(const std::tm& time) const;

    virtual ~IScheduledTransferService() = default;

private:
    [[nodiscard]] virtual std::optional<ScheduledTransfer> doGetScheduledTransferById(int id) const = 0;
    [[nodiscard]] virtual std::vector<ScheduledTransfer> doGetAllScheduledTransfersFromCardId(int id) const = 0;

    virtual void doCreateScheduledTransfer(const ScheduledTransfer& transfer) const = 0;
    virtual void doUpdateScheduledTransfer(ScheduledTransfer transfer) const = 0;
    virtual void doDeleteScheduledTransfer(int id) const = 0;

    virtual void doExecuteAllScheduledTransfersByNow(const std::tm& time) const = 0;

};

inline std::optional<ScheduledTransfer> IScheduledTransferService::getScheduledTransferById(const int id) const{ return doGetScheduledTransferById(id);}
inline std::vector<ScheduledTransfer> IScheduledTransferService::getAllScheduledTransfersFromCardId(const int id) const{ return doGetAllScheduledTransfersFromCardId(id);}

inline void IScheduledTransferService::createScheduledTransfer(const ScheduledTransfer& transfer) const{ doCreateScheduledTransfer(transfer); }
inline void IScheduledTransferService::updateScheduledTransfer(const ScheduledTransfer& transfer) const{ doUpdateScheduledTransfer(transfer); }
inline void IScheduledTransferService::deleteScheduledTransfer(const int id) const{ doDeleteScheduledTransfer(id);}

inline void IScheduledTransferService::executeAllScheduledTransfersByNow(const std::tm &time) const { doExecuteAllScheduledTransfersByNow(time);}

