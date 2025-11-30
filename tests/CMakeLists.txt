add_executable(core_tests
        main.cpp
        user/UserDaoTest.cpp
        user/UserServiceTest.cpp
        cardDesign/CardDesignDaoTest.cpp
        cardDesign/CardDesignServiceTest.cpp
        currency/CurrencyDaoTest.cpp
        currency/CurrencyServiceTest.cpp
        exchangeRate/ExchangeRateDaoTest.cpp
        exchangeRate/ExchangeRateServiceTest.cpp
        card/CardDaoTest.cpp
        card/CardServiceTest.cpp
        transaction/BankTransactionDaoTest.cpp
        transaction/BankTransactionServiceTest.cpp
        scheduledTransfer/ScheduledTransferDaoTest.cpp
        scheduledTransfer/ScheduledTransferServiceTest.cpp
        common/utils/BankTransactionTestUtils.cpp
        common/utils/CardDesignTestUtils.cpp
        common/utils/CardTestUtils.cpp
        common/utils/CurrencyTestUtils.cpp
        common/utils/ExchangeRateTestUtils.cpp
        common/utils/ScheduledTransferTestUtils.cpp
        common/utils/UserTestUtils.cpp
)

target_link_libraries(core_tests PRIVATE
        core_lib
        soci_core
        soci_sqlite3
)

target_include_directories(core_tests PRIVATE
        ${CMAKE_SOURCE_DIR}/external
        ${CMAKE_CURRENT_SOURCE_DIR}/common/fixtures
        ${CMAKE_CURRENT_SOURCE_DIR}/common/utils
)

enable_testing()
add_test(NAME core_tests COMMAND core_tests)