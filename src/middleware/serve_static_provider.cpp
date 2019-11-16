#include "expresscpp/middleware/serve_static_provider.hpp"

#include <filesystem>

#include "expresscpp/console.hpp"
#include "expresscpp/impl/session.hpp"
#include "expresscpp/impl/utils.hpp"

namespace expresscpp {

StaticFileProvider::StaticFileProvider(const std::filesystem::path path_to_root_folder)
    : path_to_root_folder_(path_to_root_folder) {
  const std::string path_string = path_to_root_folder_.string();
  Console::Debug(fmt::format(R"(created static file provider for path "{}")", path_string));
}

void StaticFileProvider::UsePrefix(std::string_view prefix_path) {
  path_to_root_folder_ = prefix_path;
}

void StaticFileProvider::HandleRequests(request_t req, response_t res, next_t next) {
  assert(req);
  assert(res);

  Console::Debug("handle file response");

  std::filesystem::path requested_path;
  const std::filesystem::path requested_sub_path = req->getPath();
  if (req->getPath().empty() || req->getPath() == "/") {
    requested_path = path_to_root_folder_;
  } else if (req->getPath()[0] == '/') {
    // https://stackoverflow.com/questions/55214156/why-does-stdfilesystempathappend-replace-the-current-path-if-p-starts-with
    requested_path = path_to_root_folder_.string() + std::string(req->getPath());
  } else {
    requested_path = path_to_root_folder_ / requested_sub_path;
  }

  if (std::filesystem::exists(requested_path)) {
    Console::Debug(fmt::format(R"(file: "{}" exists)", requested_path.string()));
  } else {
    Console::Debug(fmt::format(R"(file: "{}" does not exists)", requested_path.string()));
    
    // res->SetStatus(static_cast<uint16_t>(boost::beast::http::status::not_found));
    // res->Send("not found");

    // go to next middleware, eventually 404 responder
    return next();
    // return;
  }

  // Request path must be absolute and not contain "..".
  if (req->getPath().empty() || req->getPath()[0] != '/' || req->getPath().find("..") != std::string_view::npos) {
    res->SetStatus(static_cast<uint16_t>(boost::beast::http::status::bad_request));
    res->Send("Illegal request-target");
    return;
  }

  // Build the path to the requested file
  std::string path = requested_path.string();
  Console::Debug(fmt::format(R"(accessing file: "{}")", path));

  if (req->getPath().back() == '/') {
    path = (std::filesystem::path(path) / std::filesystem::path("index.html")).string();
  }

  Console::Debug(fmt::format(R"(accessing file: "{}")", path));

  // Attempt to open the file
  boost::beast::error_code ec;
  boost::beast::http::file_body::value_type body;
  body.open(path.c_str(), boost::beast::file_mode::scan, ec);

  // Handle the case where the file doesn't exist
  if (ec == boost::beast::errc::no_such_file_or_directory) {
    //     res->SetStatus(static_cast<uint16_t>(boost::beast::http::status::not_found));
    //     res->Send("not found");

    return next();
  }

  // Handle an unknown error
  if (ec) {
    res->SetStatus(static_cast<uint16_t>(boost::beast::http::status::internal_server_error));
    res->Send(ec.message());
    return;
  }

  // Cache the size since we need it after the move
  auto const size = body.size();

  // Respond to HEAD request
  if (req->getMethod() == HttpMethod::Head) {
    res->res.set(boost::beast::http::field::content_type, mime_type(path));
    res->res.content_length(size);
    res->SendInternal();
    return;
  }

  std::ifstream t(path.c_str());
  std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

  res->res.set(boost::beast::http::field::content_type, mime_type(path));
  res->res.body() = str;

  if (res->response_sent_) {
    Console::Error("ERROR: response already sent");
    return;
  }

  res->Send();
}

void StaticFileProvider::operator()(request_t req, response_t res, next_t next) {
  HandleRequests(req, res, next);
}

}  // namespace expresscpp
