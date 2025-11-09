#include "ScheduledTransferService.h"

ScheduledTransferService::ScheduledTransferService(IScheduledTransferDao& dao, IBankTransactionDao& transactionDao) :
    _scheduledTransferDao(dao),
    _transactionDao(transactionDao)
{}

std::optional<ScheduledTransfer> ScheduledTransferService::doGetScheduledTransferById(int id) const {
    return _scheduledTransferDao.getById(id);
}

std::vector<ScheduledTransfer> ScheduledTransferService::doGetAllScheduledTransfersFromCardId(int id) const {
    return _scheduledTransferDao.getByFromCardId(id);
}

void ScheduledTransferService::doCreateScheduledTransfer(const ScheduledTransfer& transfer) const {
    _scheduledTransferDao.create(transfer);
}

void ScheduledTransferService::doUpdateScheduledTransfer(ScheduledTransfer transfer) const {
    _scheduledTransferDao.update(transfer);
}

void ScheduledTransferService::doDeleteScheduledTransfer(int id) const {
    _scheduledTransferDao.deleteById(id);
}

void ScheduledTransferService::doExecuteAllScheduledTransfersByNow(const std::tm& time) const {
    const std::time_t nowTime = std::time(nullptr);
    std::tm now{};
    localtime_s(&now, &nowTime);
    auto transfers = _scheduledTransferDao.getAllActiveBeforeDate(now);

    for (const auto& t : transfers) {
        ScheduledTransfer transfer = t;
        BankTransaction transaction(0,t.nextTun.value(), "payment", t.fromCardId,t.toCardId, t.amount, t.currencyCode, t.description, t.comment, "completed");
        _transactionDao.create(transaction);
        //TODO: make not only "payment" (add field "type" to ScheduledTransfer)
        //TODO: make not only "completed" (logic to service Transfer)

        if (transfer.frequency=="daily") {
            std::time_t newT = std::mktime(&transfer.nextTun.value());
            newT += 1 * 24 * 60 * 60;
            localtime_s(&transfer.nextTun.value(), &newT);
        }
        if (transfer.frequency=="weekly") {
            std::time_t newT = std::mktime(&transfer.nextTun.value());
            newT += 7 * 24 * 60 * 60;
            localtime_s(&transfer.nextTun.value(), &newT);
        }
        if (transfer.frequency=="monthly") {
            transfer.nextTun.value().tm_mon += 1;
            std::mktime(&transfer.nextTun.value());
        }
        updateScheduledTransfer(t);

    }
}