#include "TransactionStrategy.h"
#include "CurrencyService.h"
#include "CardService.h"

std::tm getMidnight()
{
    std::time_t now = std::time(nullptr);
    std::tm local = *std::localtime(&now);
    local.tm_hour = 0;
    local.tm_min = 0;
    local.tm_sec = 0;
    std::mktime(&local);
    return local;
}

TransferStrategy::TransferStrategy(ICardService& cardService, ICurrencyService &currencyService)
    : _cardService(cardService), _currencyService(currencyService)
{}

bool TransferStrategy::doExecute(BankTransaction &tx) const
{
    Card from = _cardService.getCardById(tx.fromCardId.value()).value();
    Card to   = _cardService.getCardById(tx.toCardId.value()).value();

    int txMinor = _currencyService.toMinor(tx.currencyCode, tx.amount);
    int fromLost = _currencyService.convert(tx.currencyCode, from.currencyCode, tx.amount);
    int toReceived = _currencyService.convert(tx.currencyCode, to.currencyCode, tx.amount);

    int balance = from.balance;
    if (from.allowCredit)
        balance += from.creditLimit.value();

    int spent = _cardService.getCardSpendingsSince(from.id, getMidnight());
    int limit = from.dailyLimit - spent;

    if (balance < fromLost || limit < fromLost) {
        tx.status = "failed";
        tx.amount = txMinor;
        return false;
    }

    from.balance -= fromLost;
    to.balance += toReceived;

    _cardService.updateCard(from);
    _cardService.updateCard(to);

    tx.amount = txMinor;
    return true;
}
