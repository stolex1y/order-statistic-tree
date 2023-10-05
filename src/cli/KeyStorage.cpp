#include "KeyStorage.h"

std::size_t KeyStorage::get_less_count(int key) {
    return storage.less_count(key);
}

int KeyStorage::find_order_statistic(std::size_t k) {
    if (k > storage.size() || k <= 0)
        throw std::invalid_argument("The key number must be greater than zero, "
                                    "but not greater than the storage size.");
    return storage.find_order_statistic(k);
}

void KeyStorage::insert_key(int key) {
    if (storage.contains(key))
        throw std::invalid_argument("The key already exists. Try something different.");
    storage.insert(key);
}
