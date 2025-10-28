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
)

target_sources(nong_bong PRIVATE
		feature/card/CardDao.cpp
		feature/cardDesign/CardDesignDao.cpp
		feature/currency/CurrencyDao.cpp
		feature/exchangeRate/ExchangeRateDao.cpp
		feature/scheduledTransfer/ScheduledTransferDao.cpp
		feature/transaction/BankTransactionDao.cpp
		feature/user/UserDao.cpp
		feature/card/CardService.cpp
)

target_link_libraries(nong_bong PRIVATE
	Qt6::Widgets
        gui_lib
        soci_core
        soci_sqlite3
)

add_custom_command(TARGET nong_bong POST_BUILD 
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		${CMAKE_SOURCE_DIR}/core/resources/database.sqlite
		$<TARGET_FILE_DIR:nong_bong>/database.sqlite 
)
