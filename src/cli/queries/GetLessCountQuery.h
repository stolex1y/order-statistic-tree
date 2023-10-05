#ifndef ORDER_STATISTIC_TREE_GETLESSCOUNTQUERY_H
#define ORDER_STATISTIC_TREE_GETLESSCOUNTQUERY_H

#include "Query.h"
#include "KeyStorage.h"

class GetLessCountQuery : public Query {
public:
    GetLessCountQuery(KeyStorage &storage, const std::string &args);

    std::string execute() override;

private:
    int key = 0;
};


#endif //ORDER_STATISTIC_TREE_GETLESSCOUNTQUERY_H
