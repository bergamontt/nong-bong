add_executable(nong_bong 
	main.cpp 
)

target_include_directories(nong_bong PRIVATE 
	card 
	cardDesign 
	currency 
	exchangeRate 
	scheduledTransfer 
	transaction 
	user 
)

target_sources(nong_bong PRIVATE
	card/CardDao.cpp
	cardDesign/CardDesignDao.cpp 
	currency/CurrencyDao.cpp
	exchangeRate/ExchangeRateDao.cpp
	scheduledTransfer/ScheduledTransferDao.cpp
	transaction/BankTransactionDao.cpp
	user/UserDao.cpp
)

target_link_libraries(nong_bong PRIVATE
        Qt6::Widgets
        gui_lib
        soci_core
        soci_sqlite3
)

add_custom_command(TARGET nong_bong POST_BUILD 
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		${CMAKE_SOURCE_DIR}/core/database.sqlite 
		$<TARGET_FILE_DIR:nong_bong>/database.sqlite 
)
