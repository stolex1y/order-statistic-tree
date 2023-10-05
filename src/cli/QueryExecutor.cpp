#include "QueryExecutor.h"

#include <sstream>
#include <stdexcept>

#include "queries/FindOrderStatisticQuery.h"
#include "queries/GetLessCountQuery.h"
#include "queries/InsertKeyQuery.h"

QueryExecutor::QueryExecutor(KeyStorage &storage) : storage(storage) {
    fill_queries();
}

std::string QueryExecutor::execute_query(const std::string &query) {
    try {
        return parse_query(query)->execute();
    } catch (const std::exception &ex) {
        return ex.what();
    }
}

void QueryExecutor::fill_queries() {
    query_names["m"] = [this](const std::string &args) {
        return std::make_unique<FindOrderStatisticQuery>(storage, args);
    };
    query_names["n"] = [this](const std::string &args) {
        return std::make_unique<GetLessCountQuery>(storage, args);
    };
    query_names["k"] = [this](const std::string &args) {
        return std::make_unique<InsertKeyQuery>(storage, args);
    };
}

std::unique_ptr<Query> QueryExecutor::parse_query(const std::string &query_str) const {
    std::stringstream stream(query_str);
    std::string name;
    std::string args;
    stream >> name >> args;
    auto query_constructor = query_names.find(name);
    if (query_constructor != query_names.end()) {
        return query_constructor->second(args);
    } else {
        throw std::invalid_argument("Unknown query.");
    }
}