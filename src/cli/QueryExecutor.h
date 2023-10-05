#ifndef ORDER_STATISTIC_TREE_QUERYEXECUTOR_H
#define ORDER_STATISTIC_TREE_QUERYEXECUTOR_H

#include <functional>
#include <memory>

#include "queries/Query.h"

class QueryExecutor {
public:
    explicit QueryExecutor(KeyStorage &storage);

    std::string execute_query(const std::string &query);

private:
    KeyStorage &storage;
    std::unordered_map<
            std::string,
            std::function<std::unique_ptr<Query>(const std::string &)>
    > query_names;

    void fill_queries();

    std::unique_ptr<Query> parse_query(const std::string &query_str) const;
};


#endif //ORDER_STATISTIC_TREE_QUERYEXECUTOR_H
