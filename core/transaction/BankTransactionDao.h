#pragma once
#include <soci/soci.h>
#include "BankTransaction.h"
#include "IBankTransactionDao.h"

class BankTransactionDao
{
private:
    soci::connection_pool& _pool;

    BankTransaction doGetById(int id) const;
    std::vector<BankTransaction> doGetByFromCardId(int id) const;
    void doCreateById(const BankTransaction& transfer) const;

public:
    BankTransactionDao(soci::connection_pool& pool) : _pool(pool) {}
    BankTransactionDao(const BankTransactionDao&) = delete;
    BankTransactionDao& operator=(const BankTransactionDao&) = delete;
};