#include "TransactionStrategyFactory.h"
#include "CardDao.h"

TransactionStrategyFactory::TransactionStrategyFactory(ICardDao &cardDao, ICurrencyService &currencyService)
    : _cardDao(cardDao), _currencyService(currencyService)
{}

std::unique_ptr<ITransactionStrategy> TransactionStrategyFactory::create(const std::string &type) const
{
    if (type == "withdrawal")
        return std::make_unique<WithdrawalStrategy>(_cardDao, _currencyService);
    if (type == "deposit")
        return std::make_unique<DepositStrategy>(_cardDao, _currencyService);
    if (type == "transfer")
        return std::make_unique<TransferStrategy>(_cardDao, _currencyService);
    if (type == "payment")
        return std::make_unique<PaymentStrategy>(_cardDao, _currencyService);
    return nullptr;
}
