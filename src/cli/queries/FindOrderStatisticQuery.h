#ifndef ORDER_STATISTIC_TREE_FINDORDERSTATISTICQUERY_H
#define ORDER_STATISTIC_TREE_FINDORDERSTATISTICQUERY_H

#include "KeyStorage.h"
#include "Query.h"

class FindOrderStatisticQuery : public Query {
public:
    FindOrderStatisticQuery(KeyStorage &storage, const std::string &args);

    std::string execute() override;

private:
    ssize_t k = 0;
};


#endif //ORDER_STATISTIC_TREE_FINDORDERSTATISTICQUERY_H
