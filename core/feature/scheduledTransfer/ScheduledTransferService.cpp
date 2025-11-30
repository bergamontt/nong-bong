#include "ScheduledTransferService.h"

ScheduledTransferService::ScheduledTransferService(IScheduledTransferDao& dao, IBankTransactionDao& transactionDao, IBankTransactionService& bankTransactionService, ICurrencyService &currencyService)
    : _scheduledTransferDao(dao)
    , _transactionDao(transactionDao)
    , _bankTransactionService(bankTransactionService)
    ,_currencyService(currencyService)
{}

std::optional<ScheduledTransfer> ScheduledTransferService::doGetScheduledTransferById(const int id) const
{
    return _scheduledTransferDao.getById(id);
}

std::vector<ScheduledTransfer> ScheduledTransferService::doGetAllScheduledTransfersFromCardId(const int id) const
{
    return _scheduledTransferDao.getByFromCardId(id);
}

void ScheduledTransferService::doCreateScheduledTransfer(ScheduledTransfer& transfer) const
{
    const int txMinor = _currencyService.toMinor(transfer.currencyCode, transfer.amount);
    transfer.amount = txMinor;
    _scheduledTransferDao.create(transfer);
}

void ScheduledTransferService::doUpdateScheduledTransfer(const ScheduledTransfer& transfer) const
{
    _scheduledTransferDao.update(transfer);
}

void ScheduledTransferService::doDeleteScheduledTransfer(const int id) const
{
    _scheduledTransferDao.deleteById(id);
}

void ScheduledTransferService::doExecuteAllScheduledTransfersByNow(const std::tm& time) const
{
    for (auto& transfer : _scheduledTransferDao.getAllActiveBeforeDate(time))
    {
        executeTransfer(transfer);
        scheduleNextTransfer(transfer);
    }
}

void ScheduledTransferService::executeTransfer(const ScheduledTransfer& transfer) const
{
    BankTransaction transaction;
    transaction.createdAt = transfer.nextRun.value();
    transaction.type = "payment";
    transaction.fromCardId = transfer.fromCardId;
    transaction.toCardId = transfer.toCardId;
    transaction.amount = transfer.amount;
    transaction.currencyCode = transfer.currencyCode;
    transaction.description = transfer.description;
    transaction.comment = transfer.comment;
    transaction.status = "completed";
    _bankTransactionService.createBankTransaction(transaction);
}

void ScheduledTransferService::scheduleNextTransfer(ScheduledTransfer& transfer) const
{
    if (!transfer.nextRun.has_value())
        return;
    transfer.nextRun = calculateNextRun(transfer.nextRun.value(), transfer.frequency);
    updateScheduledTransfer(transfer);
}

std::tm ScheduledTransferService::calculateNextRun(const std::tm& curr, const std::string& freq) const
{
    std::tm next = curr;
    std::time_t t = std::mktime(&next);

    if (freq == "daily")
        t += 1 * 24 * 60 * 60;
    else if (freq == "weekly")
        t += 7 * 24 * 60 * 60;
    else if (freq == "monthly")
    {
        next.tm_mon += 1;
        t = std::mktime(&next);
    }

    if (freq != "monthly")
        if (const auto tmp = std::localtime(&t))
            next = *tmp;

    return next;
}