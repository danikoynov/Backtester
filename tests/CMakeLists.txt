include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(unit_tests
    execution/test_execution_model.cpp
    execution/test_fill.cpp
    execution/test_order_book.cpp
    domain/test_order.cpp
    domain/test_portfolio.cpp
    domain/test_ticker.cpp
    engine/test_action.cpp
    engine/test_broker.cpp
    engine/test_engine.cpp
    data/test_bar.cpp
    data/test_detail.cpp
    data/test_data_fetcher.cpp
    strategy/test_strategy.cpp
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