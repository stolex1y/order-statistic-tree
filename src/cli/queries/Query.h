#ifndef ORDER_STATISTIC_TREE_QUERY_H
#define ORDER_STATISTIC_TREE_QUERY_H

#include <string>

#include "KeyStorage.h"

class Query {
public:
    explicit Query(KeyStorage &storage);

    virtual ~Query() = default;

    virtual std::string execute();

protected:
    KeyStorage &storage;
};

#endif //ORDER_STATISTIC_TREE_QUERY_H
