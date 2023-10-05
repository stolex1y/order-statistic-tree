#ifndef ORDER_STATISTIC_TREE_INSERTKEYQUERY_H
#define ORDER_STATISTIC_TREE_INSERTKEYQUERY_H

#include "Query.h"
#include "KeyStorage.h"

class InsertKeyQuery : public Query {
public:
    InsertKeyQuery(KeyStorage &storage, const std::string &args);

    std::string execute() override;

private:
    int key = 0;
};

#endif //ORDER_STATISTIC_TREE_INSERTKEYQUERY_H
