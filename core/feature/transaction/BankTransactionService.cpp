#include "BankTransactionService.h"
#include "ICurrencyDao.h"
#include "TransactionValidator.h"

BankTransactionService::BankTransactionService(
    IBankTransactionDao &dao, ICardService &cardService, ICurrencyService &currencyService
)
    : _bankTransactionDao(dao)
    , _cardService(cardService)
    , _currencyService(currencyService)
    , _factory(_cardService, _currencyService)
{}

std::optional<BankTransaction> BankTransactionService::doGetBankTransactionById(const int id) const
{
    return _bankTransactionDao.getById(id);
}

std::vector<BankTransaction> BankTransactionService::doGetAllBankTransactionsFromCardId(const int id) const
{
    return _bankTransactionDao.getByFromCardId(id);
}

std::vector<BankTransaction> BankTransactionService::doGetBankTransactionByUserCardId(const int id) const
{
    return _bankTransactionDao.getByUserCardId(id);
}

bool BankTransactionService::doCreateBankTransaction(BankTransaction &tx)
{
    if (!TransactionValidator::validateCardIdsRule(tx)) {
        tx.status = "failed";
        return false;
    }

    const auto strategy = _factory.create(tx.type);
    if (!strategy) {
        tx.status = "failed";
        return false;
    }

    const bool ok = strategy->execute(tx);
    _bankTransactionDao.create(tx);
    return ok;
}