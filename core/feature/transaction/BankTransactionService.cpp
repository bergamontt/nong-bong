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

bool BankTransactionService::doCreateBankTransaction(const BankTransaction& transaction) const {
    if (true) {
        _bankTransactionDao.create(transaction);
        return true;
    }
    return false;
}
