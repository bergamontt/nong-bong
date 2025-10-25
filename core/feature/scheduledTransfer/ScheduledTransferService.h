#pragma once
#include <vector>
#include "IScheduledTransferDao.h"
#include "IScheduledTransferService.h"
#include "ScheduledTransfer.h"

class ScheduledTransferService final : public IScheduledTransferService
{
    std::shared_ptr<IScheduledTransferDao> dao;

    explicit ScheduledTransferService(std::shared_ptr<IScheduledTransferDao> dao);

    [[nodiscard]] std::optional<ScheduledTransfer> doGetScheduledTransferById(int id) const override;
    [[nodiscard]] std::vector<ScheduledTransfer> doGetAllScheduledTransfersFromCardId(int id) const override;

    void doCreateScheduledTransfer(const ScheduledTransfer& transfer) const override;
    void doUpdateScheduledTransfer(ScheduledTransfer transfer) const override;
    void doDeleteScheduledTransfer(int id) const override;

    void doExecuteAllScheduledTransfersByNow(const std::tm& time) const override;

};

