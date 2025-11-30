#pragma once

class BankTransaction;

class TransactionValidator
{
public:
    static bool validateCardIdsRule( const BankTransaction& tx);
};