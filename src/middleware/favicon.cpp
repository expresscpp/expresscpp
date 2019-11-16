#include "expresscpp/middleware/favicon.hpp"

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
    std::ostringstream out;
    for (char c : favicon_ico_) {
      out << c;
    }

    std::string s(out.str());
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

}  // namespace expresscpp
