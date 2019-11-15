#pragma once

#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "boost/asio/ip/tcp.hpp"
#include "boost/asio/strand.hpp"
#include "boost/beast/core.hpp"
#include "boost/beast/http.hpp"
#include "boost/beast/version.hpp"
#include "boost/config.hpp"
#include "boost/uuid/uuid.hpp"
#include "expresscpp/key.hpp"
#include "expresscpp/options.hpp"

namespace expresscpp {


std::string boostUUIDToString(const boost::uuids::uuid& uuid);

std::string getFileName(const std::string& s);

// Return a reasonable mime type based on the extension of a file.
boost::beast::string_view mime_type(boost::beast::string_view path);

}  // namespace expresscpp
