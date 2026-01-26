# Creates a library with given files
add_library(bt_core
    domain/order.cpp
    domain/ticker.cpp
    domain/portfolio.cpp
    data/data_fetcher.cpp
    data/detail.cpp
    data/bar.cpp
    execution/order_book.cpp
    execution/fill.cpp
    execution/execution_model.cpp
)

# Every time some file uses the library,
# the library can search in given file for includes

target_include_directories(bt_core
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)