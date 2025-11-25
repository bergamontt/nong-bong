#include "TransactionStrategy.h"
#include "CurrencyService.h"
#include "CardDao.h"

TransferStrategy::TransferStrategy(ICardDao &cardDao, ICurrencyService &currencyService)
    : _cardDao(cardDao), _currencyService(currencyService)
{}

bool TransferStrategy::doExecute(BankTransaction &tx) const
{
    Card from = _cardDao.getById(tx.fromCardId.value()).value();
    Card to   = _cardDao.getById(tx.toCardId.value()).value();

    int txMinor = _currencyService.toMinor(tx.currencyCode, tx.amount);
    int fromLost = _currencyService.convert(tx.currencyCode, from.currencyCode, tx.amount);
    int toReceived = _currencyService.convert(tx.currencyCode, to.currencyCode, tx.amount);

    int allowed = from.balance;
    if (from.allowCredit)
        allowed += from.creditLimit.value();

    if (allowed < fromLost) {
        tx.status = "failed";
        tx.amount = txMinor;
        return false;
    }

    from.balance -= fromLost;
    to.balance += toReceived;

    _cardDao.update(from);
    _cardDao.update(to);

    tx.amount = txMinor;
    return true;
}
