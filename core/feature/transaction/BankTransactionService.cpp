#include "BankTransactionService.h"

#include <iostream>

#include "ICurrencyDao.h"

BankTransactionService::BankTransactionService(
    IBankTransactionDao &dao, ICardDao &cardDao, ICurrencyService &currencyService
)
    : _bankTransactionDao(dao)
    , _cardDao(cardDao)
    , _currencyService(currencyService)
    , _factory(_cardDao, _currencyService)
{}

std::optional<BankTransaction> BankTransactionService::doGetBankTransactionById(const int id) const {
    return _bankTransactionDao.getById(id);
}

std::vector<BankTransaction> BankTransactionService::doGetAllBankTransactionsFromCardId(const int id) const {
    return _bankTransactionDao.getByFromCardId(id);
}

std::vector<BankTransaction> BankTransactionService::doGetBankTransactionByUserCardId(const int id) const {
    return _bankTransactionDao.getByUserCardId(id);
}

bool BankTransactionService::doCreateBankTransaction(BankTransaction &tx) {
    if (!validateCardIdsRule(tx)) {
        tx.status = "failed";
        return false;
    }

    const auto strategy = _factory.create(tx.type);
    if (!strategy) {
        tx.status = "failed";
        return false;
    }

    const bool ok = strategy->execute(tx);
    _bankTransactionDao.create(tx);
    return ok;
}

bool BankTransactionService::validateCardIdsRule(const BankTransaction &tx) {
    if (tx.type == "withdrawal") {
        return tx.fromCardId.has_value() && !tx.toCardId.has_value();
    }
    if (tx.type == "deposit") {
        return !tx.fromCardId.has_value() && tx.toCardId.has_value();
    }
    if (tx.type == "transfer" || tx.type == "payment") {
        return tx.fromCardId.has_value() && tx.toCardId.has_value();
    }
    return false;
}