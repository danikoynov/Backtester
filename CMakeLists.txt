cmake_minimum_required(VERSION 3.14)
project(backtester LANGUAGES CXX)

# Sets the standard version of C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CTest)
enable_testing()

# Goes into these subdirectories, adds their contents
# and then runs their respective CMake files
add_subdirectory(src)
add_subdirectory(tests)