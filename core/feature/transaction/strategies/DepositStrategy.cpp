#include "TransactionStrategy.h"
#include "CurrencyService.h"
#include "CardService.h"

DepositStrategy::DepositStrategy(ICardService& cardService, ICurrencyService &currencyService)
    : _cardService(cardService), _currencyService(currencyService)
{}

bool DepositStrategy::doExecute(BankTransaction &tx) const
{
    Card to = _cardService.getCardById(tx.toCardId.value()).value();

    const int txMinor = _currencyService.toMinor(tx.currencyCode, tx.amount);
    const int toReceived = _currencyService.convert(tx.currencyCode, to.currencyCode, tx.amount);

    to.balance += toReceived;
    _cardService.updateCard(to);

    tx.amount = txMinor;
    return true;
}
