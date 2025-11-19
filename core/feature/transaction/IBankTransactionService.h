#pragma once
#include <vector>
#include "BankTransaction.h"

class IBankTransactionService
{
public:
    std::optional<BankTransaction> getBankTransactionById(int id) const;
    std::vector<BankTransaction> getAllBankTransactionsFromCardId(int id) const;
    std::vector<BankTransaction> getBankTransactionByUserCardId(int id) const;
    bool createBankTransaction(BankTransaction& transaction);
    virtual ~IBankTransactionService() = default;
private:
    virtual std::optional<BankTransaction> doGetBankTransactionById(int id) const = 0;
    virtual std::vector<BankTransaction> doGetAllBankTransactionsFromCardId(int id) const = 0;
    virtual std::vector<BankTransaction> doGetBankTransactionByUserCardId(int id) const = 0;
    virtual bool doCreateBankTransaction(BankTransaction& transaction) = 0;
};

inline std::optional<BankTransaction> IBankTransactionService::getBankTransactionById(const int id) const
{
    return doGetBankTransactionById(id);
}

inline std::vector<BankTransaction> IBankTransactionService::getAllBankTransactionsFromCardId(const int id) const
{
    return doGetAllBankTransactionsFromCardId(id);
}

inline std::vector<BankTransaction> IBankTransactionService::getBankTransactionByUserCardId(const int id) const
{
    return doGetBankTransactionByUserCardId(id);
}

inline bool IBankTransactionService::createBankTransaction(BankTransaction& transaction)
{
    return doCreateBankTransaction(transaction);
}