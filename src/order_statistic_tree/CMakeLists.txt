set(TARGET_LIB lib_order_statistic_tree)

add_library(
        ${TARGET_LIB}
        INTERFACE
        include/OrderStatisticTree.h
)
target_include_directories(${TARGET_LIB} INTERFACE include)
