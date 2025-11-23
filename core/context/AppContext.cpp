#include "AppContext.h"
#include <soci/sqlite3/soci-sqlite3.h>

using namespace std;

AppContext::AppContext()
    : _dataSource(make_unique<DataSource>("database.sqlite", soci::sqlite3))
    , _cardDao(make_unique<CardDao>(_dataSource->pool()))
    , _cardDesignDao(make_unique<CardDesignDao>(_dataSource->pool()))
    , _currencyDao(make_unique<CurrencyDao>(_dataSource->pool()))
    , _exchangeRateDao(make_unique<ExchangeRateDao>(_dataSource->pool()))
    , _scheduledTransferDao(make_unique<ScheduledTransferDao>(_dataSource->pool()))
    , _bankTransactionDao(make_unique<BankTransactionDao>(_dataSource->pool()))
    , _userDao(make_unique<UserDao>(_dataSource->pool()))
    , _cardService(make_unique<CardService>(*_cardDao))
    , _cardDesignService(make_unique<CardDesignService>(*_cardDesignDao))
    , _exchangeRateService(make_unique<ExchangeRateService>(*_exchangeRateDao))
    , _currencyService(make_unique<CurrencyService>(*_currencyDao, *_exchangeRateDao))
    , _bankTransactionService(make_unique<BankTransactionService>(*_bankTransactionDao, *_cardDao, *_currencyService))
    , _scheduledTransferService(make_unique<ScheduledTransferService>(*_scheduledTransferDao, *_bankTransactionDao, *_bankTransactionService))
    , _userService(make_unique<UserService>(*_userDao))
{}
