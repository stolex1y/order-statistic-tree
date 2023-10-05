#include <iostream>
#include <variant>
#include <stdexcept>
#include <optional>

#include "KeyStorage.h"
#include "QueryExecutor.h"

int run() {
    using std::cin, std::cout, std::endl;
    KeyStorage storage;
    QueryExecutor executor(storage);
    std::string query;
    while (getline(std::cin, query)) {
        std::cout << executor.execute_query(query) << std::endl;
    }
    return 0;
}