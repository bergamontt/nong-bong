#include "TransactionStrategy.h"
#include "CurrencyService.h"
#include "CardService.h"

std::tm getMidnight();

WithdrawalStrategy::WithdrawalStrategy(ICardService& cardService, ICurrencyService &currencyService)
    : _cardService(cardService), _currencyService(currencyService)
{}

bool WithdrawalStrategy::doExecute(BankTransaction &tx) const
{
    Card from = _cardService.getCardById(tx.fromCardId.value()).value();

    const int txMinor = _currencyService.toMinor(tx.currencyCode, tx.amount);
    const int fromLost = _currencyService.convert(tx.currencyCode, from.currencyCode, tx.amount);

    int balance = from.balance;
    if (from.allowCredit)
        balance += from.creditLimit.value();

    int spent = _cardService.getCardSpendingsSince(from.id, getMidnight());
    int limit = from.dailyLimit - spent;

    if (balance < fromLost || limit < fromLost) {
        tx.status = "failed";
        return false;
    }

    from.balance -= fromLost;
    _cardService.updateCard(from);

    tx.amount = txMinor;
    return true;
}