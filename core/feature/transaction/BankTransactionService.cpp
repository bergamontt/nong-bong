#include "BankTransactionService.h"

#include <iostream>

#include "ICurrencyDao.h"

BankTransactionService::BankTransactionService(IBankTransactionDao& dao, ICardDao& cardDao, IExchangeRateDao& exchangeRateDao, ICurrencyDao& currencyDao) :
    _bankTransactionDao(dao), _cardDao(cardDao), _exchangeRateDao(exchangeRateDao), _currencyDao(currencyDao)
{}

std::optional<BankTransaction> BankTransactionService::doGetBankTransactionById(int id) const {
    return _bankTransactionDao.getById(id);
}

std::vector<BankTransaction> BankTransactionService::doGetAllBankTransactionsFromCardId(int id) const {
    return _bankTransactionDao.getByFromCardId(id);
}

bool BankTransactionService::doCreateBankTransaction(BankTransaction& transaction) {
    if (transaction.fromCardId.has_value() && !transaction.toCardId.has_value() && transaction.type=="withdrawal") {
        std::cout << "Withdrawal";
        Card from = _cardDao.getById(transaction.fromCardId.value()).value();
        if (from.currencyCode != transaction.currencyCode) {
            for (ExchangeRate e:_exchangeRateDao.getAll()) {
                //std::cout << e.id << " " << e.baseCurrency << " "<< e.targetCurrency << " "<< e.rate << " " << std::endl;
                if ((e.baseCurrency == transaction.currencyCode)&&(e.targetCurrency == from.currencyCode)) {
                    transaction.amount = transaction.amount * e.rate;
                    transaction.currencyCode = from.currencyCode;
                }
            }
        }

        for (Currency c:_currencyDao.getAll()) {
            if (c.code == transaction.currencyCode) {
                transaction.amount = transaction.amount * c.minorUnit;
            }
        }

        std::cout << std::endl << transaction.currencyCode << std::endl;
        std::cout << from.balance << " " << transaction.amount << std::endl;

        if (from.balance<transaction.amount) {
            transaction.status = "failed";
            _bankTransactionDao.create(transaction);
            return false;
        }
        from.balance -= transaction.amount;
        _cardDao.update(from);
        _bankTransactionDao.create(transaction);
        return true;
    }
    if (!transaction.fromCardId.has_value() && transaction.toCardId.has_value() && transaction.type=="deposit") {
        std::cout << "Deposit";
        Card to = _cardDao.getById(transaction.toCardId.value()).value();
        if (to.currencyCode != transaction.currencyCode) {
            for (ExchangeRate e:_exchangeRateDao.getAll()) {
                if ((e.baseCurrency == transaction.currencyCode)&&(e.targetCurrency == to.currencyCode)) {
                    transaction.amount = transaction.amount * e.rate;
                    transaction.currencyCode = to.currencyCode;
                }
            }
        }

        for (Currency c:_currencyDao.getAll()) {
            if (c.code == transaction.currencyCode) {
                transaction.amount = transaction.amount * c.minorUnit;
            }
        }

        std::cout << std::endl << transaction.currencyCode << std::endl;
        std::cout << to.balance << " " << transaction.amount << std::endl;

        to.balance += transaction.amount;
        _cardDao.update(to);
        _bankTransactionDao.create(transaction);
        return true;
    }
    if (!transaction.fromCardId.has_value() && !transaction.toCardId.has_value()) {
        std::cout << "ERROR";
        return false;
    }
    if (transaction.type=="transfer") {
        std::cout << "Transfer";
        //_bankTransactionDao.create(transaction);
        return true;
    }
    if (transaction.type=="payment") {
        std::cout << "Payment";
        //_bankTransactionDao.create(transaction);
        return true;
    }
    std::cout << "ERROR type";
    return true;
}
