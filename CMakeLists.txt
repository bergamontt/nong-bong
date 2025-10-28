    cmake_minimum_required(VERSION 4.0)
    project(nong_bong)

    set(CMAKE_CXX_STANDARD 20)

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)

    find_package(Qt6 REQUIRED COMPONENTS Widgets)

    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTORCC ON)

    include(FetchContent)

    FetchContent_Declare(
            soci
            GIT_REPOSITORY https://github.com/SOCI/soci.git
            GIT_TAG v4.1.2
    )
    FetchContent_MakeAvailable(soci)

    FetchContent_Declare(
            argon2
            GIT_REPOSITORY https://github.com/P-H-C/phc-winner-argon2.git
            GIT_TAG master
    )
    FetchContent_MakeAvailable(argon2)

    add_library(argon2_static STATIC
            ${argon2_SOURCE_DIR}/src/argon2.c
            ${argon2_SOURCE_DIR}/src/core.c
            ${argon2_SOURCE_DIR}/src/encoding.c
            ${argon2_SOURCE_DIR}/src/thread.c
            ${argon2_SOURCE_DIR}/src/blake2/blake2b.c
            ${argon2_SOURCE_DIR}/src/ref.c
    )
    target_include_directories(argon2_static PUBLIC ${argon2_SOURCE_DIR}/include)

    add_subdirectory(core)
    add_subdirectory(gui)
