#include "expresscpp/middleware/serve_favicon_provider.hpp"

#include <sstream>

namespace expresscpp {

FaviconProvider::FaviconProvider() {
  Console::Debug("FaviconProvider created");
}

FaviconProvider::~FaviconProvider() {
  Console::Debug("FaviconProvider destroyed");
}

void FaviconProvider::UseFavicon(std::vector<unsigned char> favicon_ico) {
  favicon_ico_ = favicon_ico;
}

void FaviconProvider::HandleRequest(request_t req, response_t res, next_t next) {
  if (req->getPath() == "/favicon.ico" && favicon_ico_.size() > 0) {
    std::string s;
    for (const char c : favicon_ico_) {
      s += c;
    }
    res->res.set(boost::beast::http::field::content_type, "image/vnd.microsoft.icon");

    res->Send(s);
  } else {
    next();
  }
}

void FaviconProvider::operator()(request_t req, response_t res, next_t next) {
  Console::Debug("favicon operator () called");
  return HandleRequest(req, res, next);
}

std::vector<unsigned char> FaviconProvider::GetFavicon() const {
  return favicon_ico_;
}

}  // namespace expresscpp
