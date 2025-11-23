#include "TransactionStrategy.h"
#include "CurrencyService.h"
#include "CardDao.h"

DepositStrategy::DepositStrategy(ICardDao &cardDao, ICurrencyService &currencyService)
    : _cardDao(cardDao), _currencyService(currencyService)
{}

bool DepositStrategy::doExecute(BankTransaction &tx) const
{
    Card to = _cardDao.getById(tx.toCardId.value()).value();

    const int txMinor = _currencyService.toMinor(tx.currencyCode, tx.amount);
    const int toReceived = _currencyService.convert(tx.currencyCode, to.currencyCode, tx.amount);

    to.balance += toReceived;
    _cardDao.update(to);

    tx.amount = txMinor;
    return true;
}
