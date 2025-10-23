add_executable(nong_bong
        cardDesign/CardDesign.h
        transaction/BankTransaction.h
        card/Card.h
        currency/Currency.h
        exchangeRate/ExchangeRate.h
        scheduledTransfer/ScheduledTransfer.h
        user/User.h
        main.cpp
        card/ICardDao.h
        cardDesign/ICardDesignDao.h
        currency/ICurrencyDao.h
        exchangeRate/IExchangeRate.h
        scheduledTransfer/IScheduledTransferDao.h
        transaction/IBankTransactionDao.h
        user/IUserDao.h
)

target_link_libraries(nong_bong PRIVATE
        Qt6::Widgets
        gui_lib
        soci_core
        soci_sqlite3
)