#pragma once
#include "BankTransaction.h"

class ITransactionStrategy
{
public:
    bool execute(BankTransaction& tx) const;
    virtual ~ITransactionStrategy() = default;
private:
    virtual bool doExecute(BankTransaction& tx) const = 0;
};

inline bool ITransactionStrategy::execute(BankTransaction& tx) const
{
    return doExecute(tx);
}