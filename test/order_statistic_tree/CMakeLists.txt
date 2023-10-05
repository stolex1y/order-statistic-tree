set(TEST_TARGET order_statistic_tree_test)

add_executable(
        ${TEST_TARGET}
        order_statistic_tree_test.cpp
)
target_link_libraries(${TEST_TARGET} lib_order_statistic_tree gtest_main)

include(GoogleTest)
gtest_discover_tests(${TEST_TARGET})