#pragma once
#include <vector>
#include "BankTransaction.h"
#include "IBankTransactionDao.h"
#include "IBankTransactionService.h"
#include "ICardService.h"
#include "ICurrencyService.h"
#include "TransactionStrategyFactory.h"

class BankTransactionService final : public IBankTransactionService
{
public:
    explicit BankTransactionService(IBankTransactionDao& dao, ICardService& cardService, ICurrencyService& currencyService);
    BankTransactionService(const BankTransactionService&) = delete;
    BankTransactionService& operator=(const BankTransactionService&)& = delete;

private:
    IBankTransactionDao& _bankTransactionDao;
    ICardService& _cardService;
    ICurrencyService& _currencyService;
    TransactionStrategyFactory _factory;

    std::optional<BankTransaction> doGetBankTransactionById(int id) const override;
    std::vector<BankTransaction> doGetAllBankTransactionsFromCardId(int id) const override;
    std::vector<BankTransaction> doGetBankTransactionByUserCardId(int id) const override;
    bool doCreateBankTransaction(BankTransaction& transaction) override;

    static bool validateCardIdsRule(const BankTransaction &tx);
};
