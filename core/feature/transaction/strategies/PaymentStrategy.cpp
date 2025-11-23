#include "TransactionStrategy.h"

PaymentStrategy::PaymentStrategy(ICardDao &cardDao, ICurrencyService &currencyService)
    : _cardDao(cardDao), _currencyService(currencyService)
{}

bool PaymentStrategy::doExecute(BankTransaction &tx) const
{
    const TransferStrategy transfer(_cardDao, _currencyService);
    return transfer.execute(tx);
}
