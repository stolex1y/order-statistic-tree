set(TARGET_LIB lib_cli_order_statistic_tree)
set(BOOTSTRAP_TARGET cli_order_statistic_tree_bootstrap)

add_library(
        ${TARGET_LIB}
        STATIC
        QueryExecutor.cpp
        QueryExecutor.h
        queries/Query.cpp
        queries/Query.h
        KeyStorage.cpp
        KeyStorage.h
        queries/GetLessCountQuery.cpp
        queries/GetLessCountQuery.h
        queries/FindOrderStatisticQuery.cpp
        queries/FindOrderStatisticQuery.h
        queries/InsertKeyQuery.cpp
        queries/InsertKeyQuery.h
        run.cpp
)
target_link_libraries(${TARGET_LIB} lib_order_statistic_tree)
target_include_directories(${TARGET_LIB} PRIVATE .)

add_executable(${BOOTSTRAP_TARGET} bootstrap.cpp)
target_link_libraries(${BOOTSTRAP_TARGET} ${TARGET_LIB})
