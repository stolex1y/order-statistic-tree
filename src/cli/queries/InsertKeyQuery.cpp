#include "InsertKeyQuery.h"

#include <string>
#include <stdexcept>
#include <sstream>


InsertKeyQuery::InsertKeyQuery(KeyStorage &storage, const std::string &args) :
        Query(storage) {
    std::stringstream stream(args);
    stream >> key;
    if (stream.fail())
        throw std::invalid_argument("Expected integer argument.");
}

std::string InsertKeyQuery::execute() {
    try {
        storage.insert_key(key);
        return "Successfully added.";
    } catch (const std::exception &ex) {
        return ex.what();
    }
}