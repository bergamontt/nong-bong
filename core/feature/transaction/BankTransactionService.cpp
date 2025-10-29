#include "BankTransactionService.h"

BankTransactionService::BankTransactionService(IBankTransactionDao& dao) :
    _bankTransactionDao(dao)
{}

std::optional<BankTransaction> BankTransactionService::doGetBankTransactionById(int id) const {
    return _bankTransactionDao.getById(id);
}

std::vector<BankTransaction> BankTransactionService::doGetAllBankTransactionsFromCardId(int id) const {
    return _bankTransactionDao.getByFromCardId(id);
}

void BankTransactionService::doCreateBankTransaction(const BankTransaction& transaction) const {
    _bankTransactionDao.create(transaction);
}
