#pragma once
#include <vector>
#include "BankTransaction.h"
#include "IBankTransactionDao.h"
#include "IBankTransactionService.h"

class BankTransactionService final : public IBankTransactionService
{
public:
    explicit BankTransactionService(IBankTransactionDao& dao);
    BankTransactionService(const BankTransactionService&) = delete;
    BankTransactionService& operator=(const BankTransactionService&)& = delete;

private:
    IBankTransactionDao& _bankTransactionDao;

    std::optional<BankTransaction> doGetBankTransactionById(int id) const override;
    std::vector<BankTransaction> doGetAllBankTransactionsFromCardId(int id) const override;
    void doCreateBankTransaction(const BankTransaction& transaction) const override;
};
