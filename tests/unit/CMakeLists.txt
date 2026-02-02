include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(tests
    test_parser.cpp
    ${PROJECT_SOURCE_DIR}/src/itch/parser.cpp
)

target_include_directories(tests PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(tests PRIVATE GTest::gtest_main GTest::gtest)

include(GoogleTest)
gtest_discover_tests(tests)
