#ifndef ORDER_STATISTIC_TREE_KEYSTORAGE_H
#define ORDER_STATISTIC_TREE_KEYSTORAGE_H

#include "OrderStatisticTree.h"

class KeyStorage {
public:
    std::size_t get_less_count(int key);

    int find_order_statistic(std::size_t k);

    void insert_key(int key);

private:
    OrderStatisticTree storage;
};


#endif //ORDER_STATISTIC_TREE_KEYSTORAGE_H
