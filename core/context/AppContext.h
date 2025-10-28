#pragma once
#include <memory>
#include "IContext.h"
#include "DataSource.h"
#include "CardService.h"
//#include "CardDesignService.h"
//#include "CurrencyService.h"
//#include "ExchangeRateService.h"
//#include "ScheduledTransferService.h"
//#include "BankTransactionService.h"
#include "UserService.h"
#include "CardDao.h"
#include "CardDesignDao.h"
#include "CurrencyDao.h"
#include "ExchangeRateDao.h"
#include "ScheduledTransferDao.h"
#include "BankTransactionDao.h"
#include "UserDao.h"

class AppContext final : public IContext
{
public:
    explicit AppContext();
private:
    std::unique_ptr<DataSource> _dataSource;

    std::unique_ptr<CardDao> _cardDao;
    std::unique_ptr<CardDesignDao> _cardDesignDao;
    std::unique_ptr<CurrencyDao> _currencyDao;
    std::unique_ptr<ExchangeRateDao> _exchangeRateDao;
    std::unique_ptr<ScheduledTransferDao> _scheduledTransferDao;
    std::unique_ptr<BankTransactionDao> _bankTransactionDao;
    std::unique_ptr<UserDao> _userDao;

    std::unique_ptr<CardService> _cardService;
    /*std::unique_ptr<CardDesignService> _cardDesignService;
    std::unique_ptr<CurrencyService> _currencyService;
    std::unique_ptr<ExchangeRateService> _exchangeRateService;
    std::unique_ptr<ScheduledTransferService> _scheduledTransferService;
    std::unique_ptr<BankTransactionService> _bankTransactionService;*/
    std::unique_ptr<UserService> _userService;

    ICardService& doCardService() const override;
    /*ICardDesignService& doCardDesignService() const override;
    ICurrencyService& doCurrencyService() const override;
    IExchangeRateService& doExchangeRateService() const override;
    IScheduledTransferService& doScheduledTransferService() const override;
    IBankTransactionService& doBankTransactionService() const override;*/
    IUserService& doUserService() const override;
};

inline ICardService& AppContext::doCardService() const
{
    return *_cardService;
}

/*inline ICardDesignService& AppContext::doCardDesignService() const
{
    return *_cardDesignService;
}

inline IBankTransactionService &AppContext::doBankTransactionService() const
{
    return *_bankTransactionService;
}

inline ICurrencyService &AppContext::doCurrencyService() const
{
    return *_currencyService;
}

inline IExchangeRateService &AppContext::doExchangeRateService() const
{
    return *_exchangeRateService;
}

inline IScheduledTransferService &AppContext::doScheduledTransferService() const
{
    return *_scheduledTransferService;
}
*/
inline IUserService &AppContext::doUserService() const
{
    return *_userService;
}
