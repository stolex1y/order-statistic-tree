#include "FindOrderStatisticQuery.h"

#include <string>
#include <exception>
#include <sstream>

FindOrderStatisticQuery::FindOrderStatisticQuery(KeyStorage &storage, const std::string &args) :
        Query(storage) {
    std::stringstream stream(args);
    stream >> k;
    if (stream.fail())
        throw std::invalid_argument("Expected integer argument.");
}

std::string FindOrderStatisticQuery::execute() {
    try {
        return std::to_string(storage.find_order_statistic(k));
    } catch (const std::exception &ex) {
        return ex.what();
    }
}
