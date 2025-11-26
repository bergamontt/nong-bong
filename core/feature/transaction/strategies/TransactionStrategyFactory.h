#pragma once
#include "TransactionStrategy.h"

class TransactionStrategyFactory
{
public:
    TransactionStrategyFactory(ICardService& cardService, ICurrencyService& currencyService);
    std::unique_ptr<ITransactionStrategy> create(const std::string& type) const;
private:
    ICardService& _cardService;
    ICurrencyService& _currencyService;
};