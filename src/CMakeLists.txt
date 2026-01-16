# Creates a library with given files
add_library(bt_core
    order.cpp
    portfolio.cpp
)

# Every time some file uses the library,
# the library can search in given file for includes

target_include_directories(bt_core
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)