#include "BankTransactionService.h"
#include <iostream>
#include "ICurrencyDao.h"

BankTransactionService::BankTransactionService(
    IBankTransactionDao& dao, ICardDao& cardDao, IExchangeRateDao& exchangeRateDao, ICurrencyDao& currencyDao
)
    : _bankTransactionDao(dao)
    , _cardDao(cardDao)
    , _exchangeRateDao(exchangeRateDao)
    , _currencyDao(currencyDao)
{}

std::optional<BankTransaction> BankTransactionService::doGetBankTransactionById(const int id) const
{
    return _bankTransactionDao.getById(id);
}

std::vector<BankTransaction> BankTransactionService::doGetAllBankTransactionsFromCardId(const int id) const
{
    return _bankTransactionDao.getByFromCardId(id);
}

std::vector<BankTransaction> BankTransactionService::doGetBankTransactionByUserCardId(const int id) const
{
    return _bankTransactionDao.getByUserCardId(id);
}

bool BankTransactionService::doCreateBankTransaction(BankTransaction& transaction)
{
    if (transaction.fromCardId.has_value() && !transaction.toCardId.has_value() && transaction.type=="withdrawal") {
        std::cout << "Withdrawal";
        Card from = _cardDao.getById(transaction.fromCardId.value()).value();
        
        for (const Currency& c : _currencyDao.getAll()) {
            if (c.code == transaction.currencyCode) {
                transaction.amount *= c.minorUnit;
            }
        }
        int fromLost = transaction.amount;
        if (from.currencyCode != transaction.currencyCode) {
            for (const ExchangeRate& e:_exchangeRateDao.getAll()) {
                //std::cout << e.id << " " << e.baseCurrency << " "<< e.targetCurrency << " "<< e.rate << " " << std::endl;
                if ((e.baseCurrency == transaction.currencyCode)&&(e.targetCurrency == from.currencyCode)) {
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
    if (!transaction.fromCardId.has_value() && transaction.toCardId.has_value() && transaction.type=="deposit") {
        std::cout << "Deposit";
        Card to = _cardDao.getById(transaction.toCardId.value()).value();
        
        for (Currency c : _currencyDao.getAll()) {
            if (c.code == transaction.currencyCode) {
                transaction.amount = transaction.amount * c.minorUnit;
            }
        }
        int toReceived = transaction.amount;
        if (to.currencyCode != transaction.currencyCode) {
            for (const ExchangeRate& e:_exchangeRateDao.getAll()) {
                if ((e.baseCurrency == transaction.currencyCode)&&(e.targetCurrency == to.currencyCode)) {
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
    if (!transaction.fromCardId.has_value() && !transaction.toCardId.has_value()) {
        std::cout << "ERROR";
        return false;
    }
    if (transaction.type=="transfer") {
        std::cout << "Transfer";
        Card to = _cardDao.getById(transaction.toCardId.value()).value();
        Card from = _cardDao.getById(transaction.fromCardId.value()).value();
        
        int fromLost = transaction.amount;
        for (const Currency& c : _currencyDao.getAll()) {
            if (c.code == from.currencyCode) {
                fromLost *= c.minorUnit;
            }
        }
        if (from.currencyCode != transaction.currencyCode) {
            for (const ExchangeRate& e : _exchangeRateDao.getAll()) {
                if ((e.baseCurrency == transaction.currencyCode) && (e.targetCurrency == from.currencyCode)) {
                    fromLost *= e.rate;
                }
            }
        }

        int toReceived = transaction.amount;
        for (const Currency& c : _currencyDao.getAll()) {
            if (c.code == to.currencyCode) {
                toReceived *= c.minorUnit;
            }
        }
        if (from.currencyCode != transaction.currencyCode) {
            for (const ExchangeRate& e : _exchangeRateDao.getAll()) {
                if ((e.baseCurrency == transaction.currencyCode) && (e.targetCurrency == from.currencyCode)) {
                    fromLost *= e.rate;
                }
            }
        }

        for (const Currency& c : _currencyDao.getAll()) {
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
    if (transaction.type=="payment") {
        std::cout << "Payment";
        Card to = _cardDao.getById(transaction.toCardId.value()).value();
        Card from = _cardDao.getById(transaction.fromCardId.value()).value();

        int fromLost = transaction.amount;
        for (const Currency& c : _currencyDao.getAll()) {
            if (c.code == from.currencyCode) {
                fromLost = fromLost * c.minorUnit;
            }
        }
        if (from.currencyCode != transaction.currencyCode) {
            for (const ExchangeRate& e : _exchangeRateDao.getAll()) {
                if ((e.baseCurrency == transaction.currencyCode) && (e.targetCurrency == from.currencyCode)) {
                    fromLost = fromLost * e.rate;
                }
            }
        }

        int toReceived = transaction.amount;
        for (const Currency& c : _currencyDao.getAll()) {
            if (c.code == to.currencyCode) {
                toReceived = toReceived * c.minorUnit;
            }
        }
        if (from.currencyCode != transaction.currencyCode) {
            for (const ExchangeRate& e : _exchangeRateDao.getAll()) {
                if ((e.baseCurrency == transaction.currencyCode) && (e.targetCurrency == from.currencyCode)) {
                    fromLost = fromLost * e.rate;
                }
            }
        }

        for (const Currency& c : _currencyDao.getAll()) {
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
    std::cout << "ERROR type";
    return false;
}
