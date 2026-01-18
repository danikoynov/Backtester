# Creates a library with given files
add_library(bt_core
    domain/order.cpp
    domain/ticker.cpp
    domain/portfolio.cpp
    data/data_fetcher.cpp
)

# Every time some file uses the library,
# the library can search in given file for includes

target_include_directories(bt_core
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)