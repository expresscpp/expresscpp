#include "expresscpp/router.hpp"

#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"

Router::Router() {
  timestamp_ = std::chrono::system_clock::now();
  uuid_ = boost::uuids::random_generator()();
}

Router::Router(std::string_view name) : name_(name) {
  timestamp_ = std::chrono::system_clock::now();
  uuid_ = boost::uuids::random_generator()();
}
