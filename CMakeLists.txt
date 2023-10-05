cmake_minimum_required(VERSION 3.26)
project(order_statistic_tree)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)
add_subdirectory(test)

enable_testing()