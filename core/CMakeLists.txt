add_library(core_lib
		context/AppContext.cpp
		db/DataSource.cpp

		feature/card/CardDao.cpp
		feature/card/validator/CardValidator.cpp
		feature/cardDesign/CardDesignDao.cpp
		feature/currency/CurrencyDao.cpp
		feature/exchangeRate/ExchangeRateDao.cpp
		feature/scheduledTransfer/ScheduledTransferDao.cpp
		feature/transaction/BankTransactionDao.cpp
		feature/user/UserDao.cpp

		feature/card/CardService.cpp
		feature/cardDesign/CardDesignService.cpp
		feature/currency/CurrencyService.cpp
		feature/exchangeRate/ExchangeRateService.cpp
		feature/scheduledTransfer/ScheduledTransferService.cpp
		feature/transaction/BankTransactionService.cpp
		feature/user/UserService.cpp
		security/Hasher.cpp

		feature/transaction/strategies/DepositStrategy.cpp
		feature/transaction/strategies/PaymentStrategy.cpp
		feature/transaction/strategies/TransferStrategy.cpp
		feature/transaction/strategies/WithdrawalStrategy.cpp
		feature/transaction/strategies/TransactionStrategyFactory.cpp
		feature/transaction/validator/TransactionValidator.cpp
)

target_include_directories(core_lib PUBLIC
		context
		feature/card
		feature/card/validator
		feature/cardDesign
		feature/currency
		feature/exchangeRate
		feature/scheduledTransfer
		feature/transaction
		feature/transaction/strategies
		feature/transaction/validator
		feature/user
		db
		db/sql
		security
)

target_link_libraries(core_lib PUBLIC
        soci_core
        soci_sqlite3
		argon2_static
)