# Creates a library with given files
add_library(order_lib
    order.cpp
)

# Every time some file uses the library,
# the library can search in given file for includes

target_include_directories(order_lib
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)