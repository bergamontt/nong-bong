add_executable(nong_bong
	main.cpp 
)

target_include_directories(nong_bong PRIVATE
		context
		feature/card
		feature/cardDesign
		feature/currency
		feature/exchangeRate
		feature/scheduledTransfer
		feature/transaction
		feature/user
		db
		db/sql
		security
)

target_sources(nong_bong PRIVATE
		context/AppContext.cpp
		db/DataSource.cpp


		feature/card/CardDao.cpp
		feature/cardDesign/CardDesignDao.cpp
		feature/currency/CurrencyDao.cpp
		feature/exchangeRate/ExchangeRateDao.cpp
		feature/scheduledTransfer/ScheduledTransferDao.cpp
		feature/transaction/BankTransactionDao.cpp
		feature/user/UserDao.cpp

		feature/card/CardService.cpp
		#feature/cardDesign/CardDesignService.cpp
		#feature/currency/CurrencyService.cpp
		#feature/exchangeRate/ExchangeRateService.cpp
		#feature/scheduledTransfer/ScheduledTransferService.cpp
		#feature/transaction/BankTransactionService.cpp
		feature/user/UserService.cpp
		security/Hasher.cpp
)

target_link_libraries(nong_bong PRIVATE
	Qt6::Widgets
        gui_lib
        soci_core
        soci_sqlite3
		argon2_static
)


add_custom_command(TARGET nong_bong POST_BUILD 
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		${CMAKE_SOURCE_DIR}/core/resources/database.sqlite
		$<TARGET_FILE_DIR:nong_bong>/database.sqlite 
)
