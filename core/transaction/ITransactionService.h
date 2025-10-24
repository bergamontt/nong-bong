#pragma once
#include <vector>
#include "BankTransaction.h"

class IBankTransactionService
{
public:
    std::optional<BankTransaction> getBankTransactionById(int id) const;
    std::vector<BankTransaction> getAllBankTransactionsFromCardId(int id) const;
    void createBankTransaction(const BankTransaction& transaction) const;

    virtual ~IBankTransactionService() = default;
private:
    virtual std::optional<BankTransaction> doGetBankTransactionById(int id) const = 0;
    virtual std::vector<BankTransaction> doGetAllBankTransactionsFromCardId(int id) const = 0;
    virtual void doCreateBankTransaction(const BankTransaction& transaction) const = 0;
};

inline std::optional<BankTransaction> IBankTransactionService::getBankTransactionById(const int id) const{ return doGetBankTransactionById(id); }
inline std::vector<BankTransaction> IBankTransactionService::getAllBankTransactionsFromCardId(const int id) const{ return doGetAllBankTransactionsFromCardId(id); }
inline void IBankTransactionService::createBankTransaction(const BankTransaction& transaction) const{ doCreateBankTransaction(transaction); }