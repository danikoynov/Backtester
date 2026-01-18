include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(unit_tests
    test_order.cpp
    test_portfolio.cpp
    test_data_fetcher.cpp
)

target_link_libraries(unit_tests
    PRIVATE
        bt_core
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(unit_tests
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)