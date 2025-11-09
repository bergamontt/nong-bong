#pragma once
#include <vector>
#include "BankTransaction.h"
#include "IBankTransactionDao.h"
#include "IBankTransactionService.h"
#include "ICardDao.h"
#include "ICurrencyDao.h"
#include "IExchangeRateDao.h"

class BankTransactionService final : public IBankTransactionService
{
public:
    explicit BankTransactionService(IBankTransactionDao& dao, ICardDao& cardDao, IExchangeRateDao& exchangeRateDao, ICurrencyDao& currencyDao);
    BankTransactionService(const BankTransactionService&) = delete;
    BankTransactionService& operator=(const BankTransactionService&)& = delete;

private:
    IBankTransactionDao& _bankTransactionDao;
    ICardDao& _cardDao;
    IExchangeRateDao& _exchangeRateDao;
    ICurrencyDao& _currencyDao;


    std::optional<BankTransaction> doGetBankTransactionById(int id) const override;
    std::vector<BankTransaction> doGetAllBankTransactionsFromCardId(int id) const override;
    bool doCreateBankTransaction(BankTransaction& transaction) override;
};
