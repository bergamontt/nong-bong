#pragma once
#include <soci/soci.h>
#include "BankTransaction.h"
#include "IBankTransactionDao.h"

class BankTransactionDao final : public IBankTransactionDao
{
public:
    explicit BankTransactionDao(soci::connection_pool& pool) : _pool(pool) {}
    BankTransactionDao(const BankTransactionDao&) = delete;
    BankTransactionDao& operator=(const BankTransactionDao&) = delete;
private:
    soci::connection_pool& _pool;

    BankTransaction doGetById(int id) const override;
    std::vector<BankTransaction> doGetByFromCardId(int id) const override;
    std::vector<BankTransaction> doGetByUserCardId(int cardId) const override;
    void doCreate(BankTransaction& transfer) const override;
};