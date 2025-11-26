#include "TransactionStrategyFactory.h"
#include "CardDao.h"

TransactionStrategyFactory::TransactionStrategyFactory(ICardService& cardService, ICurrencyService &currencyService)
    : _cardService(cardService), _currencyService(currencyService)
{}

std::unique_ptr<ITransactionStrategy> TransactionStrategyFactory::create(const std::string &type) const
{
    if (type == "withdrawal")
        return std::make_unique<WithdrawalStrategy>(_cardService, _currencyService);
    if (type == "deposit")
        return std::make_unique<DepositStrategy>(_cardService, _currencyService);
    if (type == "transfer")
        return std::make_unique<TransferStrategy>(_cardService, _currencyService);
    if (type == "payment")
        return std::make_unique<PaymentStrategy>(_cardService, _currencyService);
    return nullptr;
}
