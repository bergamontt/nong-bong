#pragma once
#include <vector>
#include "BankTransactionService.h"
#include "IBankTransactionDao.h"
#include "IScheduledTransferDao.h"
#include "IScheduledTransferService.h"
#include "ScheduledTransfer.h"

class ScheduledTransferService final : public IScheduledTransferService
{
public:
    explicit ScheduledTransferService(IScheduledTransferDao& dao, IBankTransactionDao& transactionDao, IBankTransactionService& bankTransactionService, ICurrencyService &currencyService);
    ScheduledTransferService(const ScheduledTransferService&) = delete;
    ScheduledTransferService& operator=(const ScheduledTransferService&)& = delete;

private:
    IScheduledTransferDao& _scheduledTransferDao;
    IBankTransactionDao& _transactionDao;
    IBankTransactionService& _bankTransactionService;
    ICurrencyService& _currencyService;

    std::optional<ScheduledTransfer> doGetScheduledTransferById(int id) const override;
    std::vector<ScheduledTransfer> doGetAllScheduledTransfersFromCardId(int id) const override;

    void doCreateScheduledTransfer(ScheduledTransfer& transfer) const override;
    void doUpdateScheduledTransfer(const ScheduledTransfer& transfer) const override;
    void doDeleteScheduledTransfer(int id) const override;

    void doExecuteAllScheduledTransfersByNow(const std::tm& time) const override;

    void executeTransfer(const ScheduledTransfer& transfer) const;
    void scheduleNextTransfer(ScheduledTransfer& transfer) const;
    std::tm calculateNextRun(const std::tm& curr, const std::string& freq) const;
};

