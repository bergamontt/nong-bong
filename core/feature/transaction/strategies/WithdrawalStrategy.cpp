#include "TransactionStrategy.h"
#include "CurrencyService.h"
#include "CardDao.h"

WithdrawalStrategy::WithdrawalStrategy(ICardDao &cardDao, ICurrencyService &currencyService)
    : _cardDao(cardDao), _currencyService(currencyService)
{}

bool WithdrawalStrategy::doExecute(BankTransaction &tx) const
{
    Card from = _cardDao.getById(tx.fromCardId.value()).value();

    const int txMinor = _currencyService.toMinor(tx.currencyCode, tx.amount);
    const int fromLost = _currencyService.convert(tx.currencyCode, from.currencyCode, tx.amount);

    int allowed = from.balance;
    if (from.allowCredit)
        allowed += from.creditLimit.value();

    if (allowed < fromLost) {
        tx.status = "failed";
        return false;
    }

    from.balance -= fromLost;
    _cardDao.update(from);

    tx.amount = txMinor;
    return true;
}