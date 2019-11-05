#include "expresscpp/impl/utils.hpp"

#include "boost/lexical_cast.hpp"
#include "boost/uuid/uuid_io.hpp"

#include "expresscpp/console.hpp"
#include "expresscpp/defines.hpp"

#define EXPRESSCPP_CONFIG_DEBUG_PATH_TO_REGEX 0

namespace expresscpp {

std::string boostUUIDToString(const boost::uuids::uuid& uuid) { return boost::lexical_cast<std::string>(uuid); }

std::string pathToRegExpString(std::string_view path, std::vector<Key>& keys, Options op) {
  std::string regex = path.data();
  if (regex == "") {
    regex = "/";
  }

  // TODO: finish this.
  //  fill in the keys -> "/api/v0/things/:id/:ip" -> ["id", "ip"];
  Key key = {.name_ = "id", .optional_ = false, .offset_ = 0};
  keys.push_back(key);

  Console::Debug(fmt::format("generating regex \"{}\" for path \"{}\"", regex, path));

#if EXPRESSCPP_CONFIG_DEBUG_PATH_TO_REGEX
  if (keys.size() > 0) {
    std::cout << "printing keys ***********************" << std::endl;
    for (const auto& k : keys) {
      std::cout << "key found : "
                << "\"" << k.name_ << "\"" << std::endl;
    }
    std::cout << "printing keys **********************" << std::endl;
  }
#endif

  return regex;
}

std::string path_cat(beast::string_view base, beast::string_view path) {
  if (base.empty()) {
    return std::string(path);
  }
  std::string result(base);
#ifdef BOOST_MSVC
  char constexpr path_separator = '\\';
  if (result.back() == path_separator) result.resize(result.size() - 1);
  result.append(path.data(), path.size());
  for (auto& c : result)
    if (c == '/') c = path_separator;
#else
  char constexpr path_separator = '/';
  if (result.back() == path_separator) {
    result.resize(result.size() - 1);
  }
  result.append(path.data(), path.size());
#endif
  return result;
}

void fail(beast::error_code ec, const char* what) { Console::Trace(fmt::format("{}:{}", what, ec.message())); }

beast::string_view mime_type(beast::string_view path) {
  using beast::iequals;
  auto const ext = [&path] {
    auto const pos = path.rfind(".");
    if (pos == beast::string_view::npos) return beast::string_view{};
    return path.substr(pos);
  }();
  if (iequals(ext, ".htm")) return "text/html";
  if (iequals(ext, ".html")) return "text/html";
  if (iequals(ext, ".php")) return "text/html";
  if (iequals(ext, ".css")) return "text/css";
  if (iequals(ext, ".txt")) return "text/plain";
  if (iequals(ext, ".js")) return "application/javascript";
  if (iequals(ext, ".json")) return "application/json";
  if (iequals(ext, ".xml")) return "application/xml";
  if (iequals(ext, ".swf")) return "application/x-shockwave-flash";
  if (iequals(ext, ".flv")) return "video/x-flv";
  if (iequals(ext, ".png")) return "image/png";
  if (iequals(ext, ".jpe")) return "image/jpeg";
  if (iequals(ext, ".jpeg")) return "image/jpeg";
  if (iequals(ext, ".jpg")) return "image/jpeg";
  if (iequals(ext, ".gif")) return "image/gif";
  if (iequals(ext, ".bmp")) return "image/bmp";
  if (iequals(ext, ".ico")) return "image/vnd.microsoft.icon";
  if (iequals(ext, ".tiff")) return "image/tiff";
  if (iequals(ext, ".tif")) return "image/tiff";
  if (iequals(ext, ".svg")) return "image/svg+xml";
  if (iequals(ext, ".svgz")) return "image/svg+xml";
  return "application/text";
}

}  // namespace expresscpp
