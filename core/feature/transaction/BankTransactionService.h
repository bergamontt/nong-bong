#pragma once
#include <vector>
#include "BankTransaction.h"
#include "IBankTransactionDao.h"
#include "IBankTransactionService.h"

class BankTransactionService final : public IBankTransactionService
{
    std::shared_ptr<IBankTransactionDao> dao;

    explicit BankTransactionService(std::shared_ptr<IBankTransactionDao> dao);

    [[nodiscard]] std::optional<BankTransaction> doGetBankTransactionById(int id) const override;
    [[nodiscard]] std::vector<BankTransaction> doGetAllBankTransactionsFromCardId(int id) const override;
    void doCreateBankTransaction(const BankTransaction& transaction) const override;
};
