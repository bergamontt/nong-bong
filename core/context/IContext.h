#pragma once

class ICardService;
class ICardDesignService;
class ICurrencyService;
class IExchangeRateService;
class IScheduledTransferService;
class IBankTransactionService;
class IUserService;

class IContext
{
public:
    ICardService& cardService() const;
    ICardDesignService& cardDesignService() const;
    ICurrencyService& currencyService() const;
    IExchangeRateService& exchangeRateService() const;
    IScheduledTransferService& scheduledTransferService() const;
    IBankTransactionService& bankTransactionService() const;
    IUserService& userService() const;
    virtual ~IContext() = default;
private:
    virtual ICardService& doCardService() const = 0;
    virtual ICardDesignService& doCardDesignService() const = 0;
    virtual ICurrencyService& doCurrencyService() const = 0;
    virtual IExchangeRateService& doExchangeRateService() const = 0;
    virtual IScheduledTransferService& doScheduledTransferService() const = 0;
    virtual IBankTransactionService& doBankTransactionService() const = 0;
    virtual IUserService& doUserService() const = 0;
};

inline ICardService &IContext::cardService() const
{
    return doCardService();
}

inline ICardDesignService &IContext::cardDesignService() const
{
    return doCardDesignService();
}

inline ICurrencyService &IContext::currencyService() const
{
    return doCurrencyService();
}

inline IExchangeRateService &IContext::exchangeRateService() const
{
    return doExchangeRateService();
}

inline IScheduledTransferService &IContext::scheduledTransferService() const
{
    return doScheduledTransferService();
}

inline IBankTransactionService &IContext::bankTransactionService() const
{
    return doBankTransactionService();
}

inline IUserService &IContext::userService() const
{
    return doUserService();
}
