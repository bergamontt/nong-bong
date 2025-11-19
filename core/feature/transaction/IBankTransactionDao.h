#pragma once
#include <vector>
#include "BankTransaction.h"

class IBankTransactionDao
{
public:
    std::optional<BankTransaction> getById(int id) const;
    std::vector<BankTransaction> getByFromCardId(int id) const;
    std::vector<BankTransaction> getByUserCardId(int cardId) const;
    void create(BankTransaction& transfer) const;
    virtual ~IBankTransactionDao() = default;
private:
    virtual std::optional<BankTransaction> doGetById(int id) const = 0;
    virtual std::vector<BankTransaction> doGetByFromCardId(int id) const = 0;
    virtual std::vector<BankTransaction> doGetByUserCardId(int cardId) const = 0;
    virtual void doCreate(BankTransaction& transfer) const = 0;
};

inline std::optional<BankTransaction> IBankTransactionDao::getById(const int id) const
{
    return doGetById(id);
}

inline std::vector<BankTransaction> IBankTransactionDao::getByFromCardId(const int id) const
{
    return doGetByFromCardId(id);
}

inline void IBankTransactionDao::create(BankTransaction& transfer) const
{
    doCreate(transfer);
}

inline std::vector<BankTransaction> IBankTransactionDao::getByUserCardId(int cardId) const
{
    return doGetByUserCardId(cardId);
}