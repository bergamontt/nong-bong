add_executable(nong_bong
        domain/CardDesign.h
        domain/BankTransaction.h
        domain/Card.h
        domain/Currency.h
        domain/ExchangeRate.h
        domain/ScheduledTransfer.h
        domain/User.h
        main.cpp
)

target_link_libraries(nong_bong PRIVATE
        Qt6::Widgets
        gui_lib
        soci_core
        soci_sqlite3
)