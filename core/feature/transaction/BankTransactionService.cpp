#include "BankTransactionService.h"
#include <iostream>
#include "ICurrencyDao.h"

BankTransactionService::BankTransactionService(
    IBankTransactionDao &dao, ICardDao &cardDao, IExchangeRateDao &exchangeRateDao, ICurrencyDao &currencyDao
)
    : _bankTransactionDao(dao)
      , _cardDao(cardDao)
      , _exchangeRateDao(exchangeRateDao)
      , _currencyDao(currencyDao) {
}

std::optional<BankTransaction> BankTransactionService::doGetBankTransactionById(const int id) const {
    return _bankTransactionDao.getById(id);
}

std::vector<BankTransaction> BankTransactionService::doGetAllBankTransactionsFromCardId(const int id) const {
    return _bankTransactionDao.getByFromCardId(id);
}

std::vector<BankTransaction> BankTransactionService::doGetBankTransactionByUserCardId(const int id) const {
    return _bankTransactionDao.getByUserCardId(id);
}

bool BankTransactionService::doCreateBankTransaction(BankTransaction &transaction) {
    if (!validateCardIdsRule(transaction)) {
        std::cout << "ERROR type";
        return false;
    }

    if (transaction.type == "withdrawal") {
        return handleWithdrawal(transaction);
    }
    if (transaction.type == "deposit") {
        return handleDeposit(transaction);
    }
    if (transaction.type == "transfer") {
        return handleTransfer(transaction);
    }
    if (transaction.type == "payment") {
        return handlePayment(transaction);
    }

    std::cout << "ERROR type";
    return false;
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

void BankTransactionService::normalizeTransactionAmount(BankTransaction &transaction) const {
    for (const Currency &c: _currencyDao.getAll()) {
        if (c.code == transaction.currencyCode) {
            transaction.amount *= c.minorUnit;
        }
    }

    for (Currency c: _currencyDao.getAll()) {
        if (c.code == transaction.currencyCode) {
            transaction.amount *= c.minorUnit;
        }
    }
}


bool BankTransactionService::handleWithdrawal(BankTransaction &transaction) const {
    std::cout << "Withdrawal";
    Card from = _cardDao.getById(transaction.fromCardId.value()).value();

    normalizeTransactionAmount(transaction);
    int fromLost = transaction.amount;
    if (from.currencyCode != transaction.currencyCode) {
        for (const ExchangeRate &e: _exchangeRateDao.getAll()) {
            if ((e.baseCurrency == transaction.currencyCode) && (e.targetCurrency == from.currencyCode)) {
                fromLost *= e.rate;
            }
        }
    }

    std::cout << std::endl << transaction.currencyCode << std::endl;
    std::cout << from.balance << " " << transaction.amount << std::endl;

    int allowed = from.balance;
    if (from.allowCredit)
        allowed += from.creditLimit.value();
    if (allowed < transaction.amount) {
        transaction.status = "failed";
        _bankTransactionDao.create(transaction);
        return false;
    }

    from.balance -= fromLost;
    _cardDao.update(from);
    _bankTransactionDao.create(transaction);
    return true;
}

bool BankTransactionService::handleDeposit(BankTransaction &transaction) const {
    std::cout << "Deposit";
    Card to = _cardDao.getById(transaction.toCardId.value()).value();

    normalizeTransactionAmount(transaction);
    int toReceived = transaction.amount;
    if (to.currencyCode != transaction.currencyCode) {
        for (const ExchangeRate &e: _exchangeRateDao.getAll()) {
            if ((e.baseCurrency == transaction.currencyCode) && (e.targetCurrency == to.currencyCode)) {
                toReceived *= e.rate;
            }
        }
    }

    std::cout << std::endl << transaction.currencyCode << std::endl;
    std::cout << to.balance << " " << transaction.amount << std::endl;

    to.balance += toReceived;
    _cardDao.update(to);
    _bankTransactionDao.create(transaction);
    return true;
}

bool BankTransactionService::handleTransfer(BankTransaction &transaction) const {
    std::cout << "Transfer";
    Card to = _cardDao.getById(transaction.toCardId.value()).value();
    Card from = _cardDao.getById(transaction.fromCardId.value()).value();

    int fromLost = transaction.amount;
    for (const Currency &c: _currencyDao.getAll()) {
        if (c.code == from.currencyCode) {
            fromLost *= c.minorUnit;
        }
    }

    if (from.currencyCode != transaction.currencyCode) {
        for (const ExchangeRate &e: _exchangeRateDao.getAll()) {
            if ((e.baseCurrency == transaction.currencyCode) && (e.targetCurrency == from.currencyCode)) {
                fromLost *= e.rate;
            }
        }
    }

    int toReceived = transaction.amount;
    for (const Currency &c: _currencyDao.getAll()) {
        if (c.code == to.currencyCode) {
            toReceived *= c.minorUnit;
        }
    }
    if (from.currencyCode != transaction.currencyCode) {
        for (const ExchangeRate &e: _exchangeRateDao.getAll()) {
            if ((e.baseCurrency == transaction.currencyCode) && (e.targetCurrency == from.currencyCode)) {
                fromLost *= e.rate;
            }
        }
    }

    for (const Currency &c: _currencyDao.getAll()) {
        if (c.code == transaction.currencyCode) {
            transaction.amount = transaction.amount * c.minorUnit;
        }
    }

    std::cout << std::endl << transaction.currencyCode << std::endl;
    std::cout << to.balance << " " << transaction.amount << std::endl;

    int allowed = from.balance;
    if (from.allowCredit)
        allowed += from.creditLimit.value();
    if (allowed < transaction.amount) {
        transaction.status = "failed";
        _bankTransactionDao.create(transaction);
        return false;
    }

    to.balance += toReceived;
    from.balance -= fromLost;
    _cardDao.update(to);
    _cardDao.update(from);
    _bankTransactionDao.create(transaction);
    return true;
}

bool BankTransactionService::handlePayment(BankTransaction &transaction) const {
    std::cout << "Payment";
    Card to = _cardDao.getById(transaction.toCardId.value()).value();
    Card from = _cardDao.getById(transaction.fromCardId.value()).value();

    int fromLost = transaction.amount;
    for (const Currency &c: _currencyDao.getAll()) {
        if (c.code == from.currencyCode) {
            fromLost = fromLost * c.minorUnit;
        }
    }
    if (from.currencyCode != transaction.currencyCode) {
        for (const ExchangeRate &e: _exchangeRateDao.getAll()) {
            if ((e.baseCurrency == transaction.currencyCode) && (e.targetCurrency == from.currencyCode)) {
                fromLost = fromLost * e.rate;
            }
        }
    }

    int toReceived = transaction.amount;
    for (const Currency &c: _currencyDao.getAll()) {
        if (c.code == to.currencyCode) {
            toReceived = toReceived * c.minorUnit;
        }
    }
    if (from.currencyCode != transaction.currencyCode) {
        for (const ExchangeRate &e: _exchangeRateDao.getAll()) {
            if ((e.baseCurrency == transaction.currencyCode) && (e.targetCurrency == from.currencyCode)) {
                fromLost = fromLost * e.rate;
            }
        }
    }

    for (const Currency &c: _currencyDao.getAll()) {
        if (c.code == transaction.currencyCode) {
            transaction.amount = transaction.amount * c.minorUnit;
        }
    }

    std::cout << std::endl << transaction.currencyCode << std::endl;
    std::cout << to.balance << " " << transaction.amount << std::endl;

    to.balance += toReceived;
    from.balance -= fromLost;
    _cardDao.update(to);
    _cardDao.update(from);
    _bankTransactionDao.create(transaction);
    return true;
}
