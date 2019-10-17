#include "expresscpp/request.hpp"

#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"

Request::Request() {
  timestamp_ = std::chrono::system_clock::now();
  uuid_ = boost::uuids::random_generator()();
}

Request::Request(std::string_view path, HttpMethod method) : path_(path), method_(method) {
  timestamp_ = std::chrono::system_clock::now();
  uuid_ = boost::uuids::random_generator()();
}
