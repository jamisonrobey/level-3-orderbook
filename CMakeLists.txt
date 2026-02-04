cmake_minimum_required(VERSION 3.30)
project(level-3-orderbook)
add_executable(level-3-orderbook src/main.cpp src/itch/parser.cpp src/fd/fd.cpp src/book/market.cpp src/book/book.cpp)

option(BUILD_UNIT_TESTS "Build unit tests" OFF)
if(BUILD_UNIT_TESTS)
    enable_testing()
    add_subdirectory(tests/unit)
endif()
