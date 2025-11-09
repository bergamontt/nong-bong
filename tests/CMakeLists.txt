add_executable(core_tests
        UserDaoTest.cpp
)

target_link_libraries(core_tests PRIVATE
        core_lib
        soci_core
        soci_sqlite3
)

target_include_directories(core_tests PRIVATE
        ${CMAKE_SOURCE_DIR}/external/doctest
)

enable_testing()
add_test(NAME core_tests COMMAND core_tests)