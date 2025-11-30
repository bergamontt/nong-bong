#pragma once
#include "Card.h"

class BankTransaction;
class CardDesign;
class Currency;
class ExchangeRate;
class ScheduledTransfer;
class User;

void assertBankTransactionEquals(const BankTransaction& expected, const BankTransaction& actual);
void assertCardDesignEquals(const CardDesign& expected, const CardDesign& actual);
void assertCardEqualsHashed(const Card& expected, const Card& actual);
void assertCardEquals(const Card& expected, const Card& actual);
void assertCurrencyEquals(const Currency& expected, const Currency& actual);
void assertExchangeRateEquals(const ExchangeRate& expected, const ExchangeRate& actual);
void assertScheduledTransferEquals(const ScheduledTransfer& expected, const ScheduledTransfer& actual);
void assertUserEqualsHashed(const User& expected, const User& actual);
void assertUserEquals(const User& expected, const User& actual);

Card makeCard(int id, std::string currency, int balance);