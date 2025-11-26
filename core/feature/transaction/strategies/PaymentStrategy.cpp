#include "TransactionStrategy.h"

PaymentStrategy::PaymentStrategy(ICardService& cardService, ICurrencyService& currencyService)
    : _cardService(cardService), _currencyService(currencyService)
{}

bool PaymentStrategy::doExecute(BankTransaction &tx) const
{
    const TransferStrategy transfer(_cardService, _currencyService);
    return transfer.execute(tx);
}
