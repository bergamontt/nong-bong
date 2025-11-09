#pragma once
#include <vector>
#include "BankTransaction.h"

class IBankTransactionDao
{
public:
    BankTransaction getById(int id) const;
    std::vector<BankTransaction> getByFromCardId(int id) const;
    void create(BankTransaction& transfer) const;
    void createWithDate(BankTransaction& transfer) const;
    virtual ~IBankTransactionDao() = default;
private:
    virtual BankTransaction doGetById(int id) const = 0;
    virtual std::vector<BankTransaction> doGetByFromCardId(int id) const = 0;
    virtual void doCreate(BankTransaction& transfer) const = 0;
    virtual void doCreateWithDate(BankTransaction& transfer) const = 0;

};

inline BankTransaction IBankTransactionDao::getById(const int id) const
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

inline void IBankTransactionDao::createWithDate(BankTransaction& transfer) const
{
    doCreateWithDate(transfer);
}