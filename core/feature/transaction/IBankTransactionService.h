#pragma once
#include <vector>
#include "BankTransaction.h"

class IBankTransactionService
{
public:
    [[nodiscard]] std::optional<BankTransaction> getBankTransactionById(int id) const;
    [[nodiscard]] std::vector<BankTransaction> getAllBankTransactionsFromCardId(int id) const;
    bool createBankTransaction(BankTransaction& transaction);

    virtual ~IBankTransactionService() = default;
private:
    [[nodiscard]] virtual std::optional<BankTransaction> doGetBankTransactionById(int id) const = 0;
    [[nodiscard]] virtual std::vector<BankTransaction> doGetAllBankTransactionsFromCardId(int id) const = 0;
    virtual bool doCreateBankTransaction(BankTransaction& transaction) = 0;
};

inline std::optional<BankTransaction> IBankTransactionService::getBankTransactionById(const int id) const{ return doGetBankTransactionById(id); }
inline std::vector<BankTransaction> IBankTransactionService::getAllBankTransactionsFromCardId(const int id) const{ return doGetAllBankTransactionsFromCardId(id); }
inline bool IBankTransactionService::createBankTransaction(BankTransaction& transaction) { return doCreateBankTransaction(transaction); }