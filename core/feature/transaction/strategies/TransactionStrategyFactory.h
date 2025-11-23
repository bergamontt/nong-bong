#pragma once
#include "TransactionStrategy.h"

class TransactionStrategyFactory
{
public:
    TransactionStrategyFactory(ICardDao& cardDao, ICurrencyService& currencyService);
    std::unique_ptr<ITransactionStrategy> create(const std::string& type) const;
private:
    ICardDao& _cardDao;
    ICurrencyService& _currencyService;
};