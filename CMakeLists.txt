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

add_subdirectory(core)
add_subdirectory(gui)
