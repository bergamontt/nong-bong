#pragma once
#include "ITransactionStrategy.h"

class ICurrencyService;
class ICardService;

class WithdrawalStrategy final : public ITransactionStrategy
{
public:
    WithdrawalStrategy(ICardService& cardService, ICurrencyService& currencyService);
private:
    ICardService& _cardService;
    ICurrencyService& _currencyService;
    bool doExecute(BankTransaction &tx) const override;
};

class DepositStrategy final : public ITransactionStrategy
{
public:
    DepositStrategy(ICardService& cardService, ICurrencyService& currencyService);
private:
    ICardService& _cardService;
    ICurrencyService& _currencyService;
    bool doExecute(BankTransaction &tx) const override;
};

class TransferStrategy final : public ITransactionStrategy
{
public:
    TransferStrategy(ICardService& cardService, ICurrencyService& currencyService);
private:
    ICardService& _cardService;
    ICurrencyService& _currencyService;
    bool doExecute(BankTransaction &tx) const override;
};

class PaymentStrategy final : public ITransactionStrategy
{
public:
    PaymentStrategy(ICardService& cardService, ICurrencyService& currencyService);
private:
    ICardService& _cardService;
    ICurrencyService& _currencyService;
    bool doExecute(BankTransaction &tx) const override;
};