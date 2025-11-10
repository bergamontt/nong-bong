add_library(gui_lib
		BankCardWidget.h
		BankCardWidget.cpp
		BankCardList.h
		BankCardList.cpp
		TransactionListModel.h
		TransactionListModel.cpp
		TransactionListWidget.h
		TransactionListWidget.cpp
		ScheduledTransferListModel.h
		ScheduledTransferListModel.cpp
		ScheduledTransferListWidget.h
		ScheduledTransferListWidget.cpp
        mainwindow.h
        mainwindow.cpp
        mainwindow.ui
)

set_target_properties(gui_lib PROPERTIES
        AUTOMOC ON
        AUTOUIC ON
        AUTORCC ON
)

target_link_libraries(gui_lib PRIVATE
		Qt6::Widgets
        soci_core
)

target_include_directories(gui_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_SOURCE_DIR}/core
		${CMAKE_SOURCE_DIR}/core/feature/card
		${CMAKE_SOURCE_DIR}/core/feature/cardDesign
		${CMAKE_SOURCE_DIR}/core/feature/currency
		${CMAKE_SOURCE_DIR}/core/feature/exchangeRate
		${CMAKE_SOURCE_DIR}/core/feature/scheduledTransfer
		${CMAKE_SOURCE_DIR}/core/feature/transaction
		${CMAKE_SOURCE_DIR}/core/feature/user
		${CMAKE_SOURCE_DIR}/core/db
		${CMAKE_SOURCE_DIR}/core/security
)
