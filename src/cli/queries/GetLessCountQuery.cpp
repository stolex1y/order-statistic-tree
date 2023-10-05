#include "GetLessCountQuery.h"

#include <string>
#include <exception>
#include <sstream>

GetLessCountQuery::GetLessCountQuery(KeyStorage &storage, const std::string &args) :
        Query(storage) {
    std::stringstream stream(args);
    stream >> key;
    if (stream.fail())
        throw std::invalid_argument("Expected integer argument.");
}

std::string GetLessCountQuery::execute() {
    try {
        return std::to_string(storage.get_less_count(key));
    } catch (const std::exception &ex) {
        return ex.what();
    }
}