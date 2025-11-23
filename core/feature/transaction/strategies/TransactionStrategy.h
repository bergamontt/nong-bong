#pragma once
#include "ITransactionStrategy.h"

class ICurrencyService;
class ICardDao;

class WithdrawalStrategy final : public ITransactionStrategy
{
public:
    WithdrawalStrategy(ICardDao& cardDao, ICurrencyService& currencyService);
private:
    ICardDao& _cardDao;
    ICurrencyService& _currencyService;
    bool doExecute(BankTransaction &tx) const override;
};

class DepositStrategy final : public ITransactionStrategy
{
public:
    DepositStrategy(ICardDao& cardDao, ICurrencyService& currencyService);
private:
    ICardDao& _cardDao;
    ICurrencyService& _currencyService;
    bool doExecute(BankTransaction &tx) const override;
};

class TransferStrategy final : public ITransactionStrategy
{
public:
    TransferStrategy(ICardDao& cardDao, ICurrencyService& currencyService);
private:
    ICardDao& _cardDao;
    ICurrencyService& _currencyService;
    bool doExecute(BankTransaction &tx) const override;
};

class PaymentStrategy final : public ITransactionStrategy
{
public:
    PaymentStrategy(ICardDao& cardDao, ICurrencyService& currencyService);
private:
    ICardDao& _cardDao;
    ICurrencyService& _currencyService;
    bool doExecute(BankTransaction &tx) const override;
};