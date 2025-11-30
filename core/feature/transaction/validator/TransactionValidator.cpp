#include "TransactionValidator.h"
#include "BankTransaction.h"

bool TransactionValidator::validateCardIdsRule(const BankTransaction& tx)
{
    if (tx.type == "withdrawal")
        return tx.fromCardId.has_value() && !tx.toCardId.has_value();
    if (tx.type == "deposit")
        return !tx.fromCardId.has_value() && tx.toCardId.has_value();
    if (tx.type == "transfer" || tx.type == "payment")
        return tx.fromCardId.has_value() && tx.toCardId.has_value();
    return false;
}
